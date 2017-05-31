#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble/ll/ll.h"
#include "../../proj/drivers/keyboard.h"
#include "../common/tl_audio.h"
#include "../common/blt_led.h"
#include "../../proj_lib/ble/trace.h"
#include "../../proj/mcu/pwm.h"
#include "../../proj_lib/ble/service/ble_ll_ota.h"
#include "../../proj/drivers/audio.h"
#include "../../proj/drivers/adc.h"
#include "../../proj/drivers/battery.h"
#include "../../proj_lib/ble/blt_config.h"
#include "../../proj_lib/ble/ble_smp.h"

#if (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8266_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)


#define  USER_TEST_BLT_SOFT_TIMER					0  //test soft timer



#define 	ADV_IDLE_ENTER_DEEP_TIME		60  //60 s
#define 	CONN_IDLE_ENTER_DEEP_TIME		60  //60 s


MYFIFO_INIT(blt_rxfifo, 64, 8);
MYFIFO_INIT(blt_txfifo, 40, 16);
////////////////////////////////////////////////////////////////////

#define			HID_HANDLE_CONSUME_REPORT			25
#define			HID_HANDLE_KEYBOARD_REPORT			29
#define			AUDIO_HANDLE_MIC					47


//////////////////////////////////////////////////////////////////////////////
//	 Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////
const u8	tbl_advData[] = {
	 0x05, 0x09, 't', 'h', 'i', 'd',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

const u8	tbl_scanRsp [] = {
		 0x08, 0x09, 't', 'R', 'e', 'm', 'o', 't', 'e',
	};

/////////////////////////// led management /////////////////////
enum{
	LED_POWER_ON = 0,
	LED_AUDIO_ON,	//1
	LED_AUDIO_OFF,	//2
	LED_SHINE_SLOW, //3
	LED_SHINE_FAST, //4
	LED_SHINE_OTA, //5
};

const led_cfg_t led_cfg[] = {
	    {1000,    0,      1,      0x00,	 },    //power-on, 1s on
	    {100,	  0 ,	  0xff,	  0x02,  },    //audio on, long on
	    {0,	      100 ,   0xff,	  0x02,  },    //audio off, long off
	    {500,	  500 ,   2,	  0x04,	 },    //1Hz for 3 seconds
	    {250,	  250 ,   4,	  0x04,  },    //2Hz for 3 seconds
	    {250,	  250 ,   200,	  0x08,  },    //2Hz for 50 seconds
};


u32		advertise_begin_tick;

u8		ui_mic_enable = 0;
u8 		key_voice_press = 0;

int 	lowBattDet_enable = 0;
int		lowBatt_alarmFlag = 0;


int     ui_mtu_size_exchange_req = 0;


//////////////////// key type ///////////////////////
#define IDLE_KEY	   			0
#define CONSUMER_KEY   	   		1
#define KEYBOARD_KEY   	   		2
#define IR_KEY   	   			3

u8 		key_type;
u8 		user_key_mode;

u8 		key_buf[8] = {0};

int 	key_not_released;

int 	ir_not_released;

u32 	latest_user_event_tick;

u8 		user_task_flg;
u8 		sendTerminate_before_enterDeep = 0;
u8 		ota_is_working = 0;



#if (STUCK_KEY_PROCESS_ENABLE)
	u32 	stuckKey_keyPressTime;
#endif




#if (REMOTE_IR_ENABLE)
	//ir key
	#define TYPE_IR_SEND			1
	#define TYPE_IR_RELEASE			2

	///////////////////// key mode //////////////////////
	#define KEY_MODE_BLE	   		0    //ble key
	#define KEY_MODE_IR        		1    //ir  key


	static const u8 kb_map_ble[49] = 	KB_MAP_BLE;  //7*7
	static const u8 kb_map_ir[49] = 	KB_MAP_IR;   //7*7
#endif



#if (BLE_REMOTE_OTA_ENABLE)
	void entry_ota_mode(void)
	{
		ota_is_working = 1;
		device_led_setup(led_cfg[LED_SHINE_OTA]);
		bls_ota_setTimeout(15 * 1000 * 1000); //set OTA timeout  15 seconds
	}



	void LED_show_ota_result(int result)
	{
		#if 0
			irq_disable();
			WATCHDOG_DISABLE;

			gpio_set_output_en(GPIO_LED, 1);

			if(result == OTA_SUCCESS){  //OTA success
				gpio_write(GPIO_LED, 1);
				sleep_us(2000000);  //led on for 2 second
				gpio_write(GPIO_LED, 0);
			}
			else{  //OTA fail

			}

			gpio_set_output_en(GPIO_LED, 0);
		#endif
	}
#endif

#if (BLE_AUDIO_ENABLE)
	u32 	key_voice_pressTick = 0;

	void		ui_enable_mic (u8 en)
	{
		ui_mic_enable = en;

		//AMIC Bias output
		gpio_set_output_en (GPIO_AMIC_BIAS, en);
		gpio_write (GPIO_AMIC_BIAS, en);

		device_led_setup(led_cfg[en ? LED_AUDIO_ON : LED_AUDIO_OFF]);


		if(en){  //audio on
			lowBattDet_enable = 0;
			battery2audio();////switch auto mode
		}
		else{  //audio off
			audio2battery();////switch manual mode
			lowBattDet_enable = 1;
		}
	}

	void voice_press_proc(void)
	{
		key_voice_press = 0;
		ui_enable_mic (1);
		if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
			ui_mtu_size_exchange_req = 0;
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
		}
	}


	void	task_audio (void)
	{
		static u32 audioProcTick = 0;
		if(clock_time_exceed(audioProcTick, 5000)){
			audioProcTick = clock_time();
		}
		else{
			return;
		}

		///////////////////////////////////////////////////////////////
		log_event(TR_T_audioTask);

		proc_mic_encoder ();

		//////////////////////////////////////////////////////////////////
		if (blc_ll_getTxFifoNumber() < 10)
		{
			int *p = mic_encoder_data_buffer ();
			if (p)					//around 3.2 ms @16MHz clock
			{
				log_event (TR_T_audioData);
				bls_att_pushNotifyData (AUDIO_HANDLE_MIC, (u8*)p, ADPCM_PACKET_LEN);
			}
		}
	}

#endif



#if (USER_TEST_BLT_SOFT_TIMER)
int gpio_test0(void)
{
	//gpio 0 toggle to see the effect
	//DBG_CHN0_TOGGLE;

	return 0;
}



int gpio_test1(void)
{
	//gpio 1 toggle to see the effect
	//DBG_CHN1_TOGGLE;

	static u8 flg = 0;
	flg = !flg;
	if(flg){
		return 7000;
	}
	else{
		return 17000;
	}

}

int gpio_test2(void)
{
	//gpio 2 toggle to see the effect
	//DBG_CHN2_TOGGLE;

	//timer last for 5 second
	if(clock_time_exceed(0, 5000000)){
		//return -1;
		//blt_soft_timer_delete(&gpio_test2);
	}
	else{

	}

	return 0;
}

int gpio_test3(void)
{
	//gpio 3 toggle to see the effect
	//DBG_CHN3_TOGGLE;

	return 0;
}



#endif


void 	ble_remote_terminate(u8 e,u8 *p, int n) //*p is terminate reason
{
	if(*p == HCI_ERR_CONN_TIMEOUT){

	}
	else if(*p == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

	}
	else if(*p == HCI_ERR_CONN_TERM_MIC_FAILURE){

	}

	 //user has push terminate pkt to ble TX buffer before deepsleep
	if(sendTerminate_before_enterDeep == 1){
		sendTerminate_before_enterDeep = 2;
	}


#if (BLE_AUDIO_ENABLE)
	if(ui_mic_enable){
		ui_enable_mic (0);
	}
#endif

	advertise_begin_tick = clock_time();

}

void	task_connect (u8 e, u8 *p, int n)
{
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  //interval=10ms latency=99 timeout=4s

	latest_user_event_tick = clock_time();

	ui_mtu_size_exchange_req = 1;
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


//This function process ...
void deep_wakeup_proc(void)
{
#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	//if deepsleep wakeup is wakeup by GPIO(key press), we must quickly scan this
	//press, hold this data to the cache, when connection established OK, send to master
	//deepsleep_wakeup_fast_keyscan
	if(analog_read(DEEP_ANA_REG0) == CONN_DEEP_FLG){
		if(kb_scan_key (KB_NUMLOCK_STATUS_POWERON, 1) && kb_event.cnt){
			deepback_key_state = DEEPBACK_KEY_CACHE;
			key_not_released = 1;
			memcpy(&kb_event_cache,&kb_event,sizeof(kb_event));
		}
	}
#endif
}





void deepback_pre_proc(int *det_key)
{
#if (DEEPBACK_FAST_KEYSCAN_ENABLE)
	// to handle deepback key cache
	if(!(*det_key) && deepback_key_state == DEEPBACK_KEY_CACHE && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN \
			&& clock_time_exceed(bls_ll_getConnectionCreateTime(), 25000)){

		memcpy(&kb_event,&kb_event_cache,sizeof(kb_event));
		*det_key = 1;

		if(key_not_released || kb_event_cache.keycode[0] == VOICE){  //no need manual release
			deepback_key_state = DEEPBACK_KEY_IDLE;
		}
		else{  //need manual release
			deepback_key_tick = clock_time();
			deepback_key_state = DEEPBACK_KEY_WAIT_RELEASE;
		}
	}
#endif
}

void deepback_post_proc(void)
{
#if (DEEPBACK_FAST_KEYSCAN_ENABLE)
	//manual key release
	if(deepback_key_state == DEEPBACK_KEY_WAIT_RELEASE && clock_time_exceed(deepback_key_tick,150000)){
		key_not_released = 0;

		key_buf[2] = 0;
		bls_att_pushNotifyData (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8); //release
		deepback_key_state = DEEPBACK_KEY_IDLE;
	}
#endif
}


void key_change_proc(void)
{

	latest_user_event_tick = clock_time();  //record latest key change time

	if(key_voice_press){  //clear voice key press flg
		key_voice_press = 0;
	}


	u8 key0 = kb_event.keycode[0];
	//u8 key1 = kb_event.keycode[1];
	u8 key_value;

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press, do  not process
	{

	}
	else if(kb_event.cnt == 1)
	{
		if(key0 == KEY_MODE_SWITCH)
		{
			user_key_mode = !user_key_mode;
			device_led_setup(led_cfg[LED_SHINE_SLOW + user_key_mode]);
		}
#if (BLE_AUDIO_ENABLE)
		else if (key0 == VOICE)
		{
			if(ui_mic_enable){  //if voice on, voice off
				//adc_clk_powerdown();
				ui_enable_mic (0);
			}
			else{ //if voice not on, mark voice key press tick
				key_voice_press = 1;
				key_voice_pressTick = clock_time();
			}
		}
#endif

#if (REMOTE_IR_ENABLE)
		else if(user_key_mode == KEY_MODE_BLE)
		{
			key_value = kb_map_ble[key0];
			if(key_value == VK_VOL_DN || key_value == VK_VOL_UP){
				key_type = CONSUMER_KEY;
				key_buf[0] = key_value == VK_VOL_UP ? 0x01 : 0x02;
				bls_att_pushNotifyData (HID_HANDLE_CONSUME_REPORT, key_buf, 2);
			}
			else
			{
				key_type = KEYBOARD_KEY;
				key_buf[2] = key_value;
				bls_att_pushNotifyData (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8);
			}

		}
		else if(user_key_mode == KEY_MODE_IR)
		{  //IR mode
			key_value = kb_map_ir[key0];
			key_type = IR_KEY;
			if(!ir_not_released){
				//ir_dispatch(TYPE_IR_SEND, 0x88, key_value);
				ir_not_released = 1;
			}
		}
		else
		{
			key_type = IDLE_KEY;
		}
#else
		else
		{
			key_value = key0;
			if(key_value == VK_VOL_DN || key_value == VK_VOL_UP){
				key_type = CONSUMER_KEY;
				key_buf[0] = key_value == VK_VOL_UP ? 0x01 : 0x02;
				bls_att_pushNotifyData (HID_HANDLE_CONSUME_REPORT, key_buf, 2);
			}
			else
			{
				key_type = KEYBOARD_KEY;
				key_buf[2] = key_value;
				bls_att_pushNotifyData (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8);
			}
		}

#endif

	}
	else   //kb_event.cnt == 0,  key release
	{
		key_not_released = 0;
		if(key_type == CONSUMER_KEY)
		{
			key_buf[0] = 0;
			bls_att_pushNotifyData (HID_HANDLE_CONSUME_REPORT, key_buf, 2);  //release
		}
		else if(key_type == KEYBOARD_KEY)
		{
			key_buf[2] = 0;
			bls_att_pushNotifyData (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8); //release
		}
#if (REMOTE_IR_ENABLE)
		else if(key_type == IR_KEY)
		{
			if(ir_not_released){
				ir_not_released = 0;
				//ir_dispatch(TYPE_IR_RELEASE, 0, 0);  //release
			}
		}
#endif
	}


}



#define GPIO_WAKEUP_KEYPROC_CNT				3

void proc_keyboard (u8 e, u8 *p, int n)
{
	static int gpioWakeup_keyProc_cnt = 0;
	static u32 keyScanTick = 0;

	//when key press gpio wakeup suspend, proc keyscan at least GPIO_WAKEUP_KEYPROC_CNT times
	//regardless of 8000 us interval
	if(e == BLT_EV_FLAG_GPIO_EARLY_WAKEUP){
		gpioWakeup_keyProc_cnt = GPIO_WAKEUP_KEYPROC_CNT;
	}
	else if(gpioWakeup_keyProc_cnt){
		gpioWakeup_keyProc_cnt --;
	}


	if(gpioWakeup_keyProc_cnt || clock_time_exceed(keyScanTick, 8000)){
		keyScanTick = clock_time();
	}
	else{
		return;
	}




	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);


#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	if(deepback_key_state != DEEPBACK_KEY_IDLE){
		deepback_pre_proc(&det_key);
	}
#endif


	if (det_key){
		key_change_proc();
	}
	

#if (BLE_AUDIO_ENABLE)
	 //long press voice 1 second
		if(key_voice_press && !ui_mic_enable && clock_time_exceed(key_voice_pressTick,1000000)){
			voice_press_proc();
		}
#endif


#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	if(deepback_key_state != DEEPBACK_KEY_IDLE){
		deepback_post_proc();
	}
#endif
}


extern u32	scan_pin_need;

int AA_dbg_deep;

//_attribute_ram_code_
void blt_pm_proc(void)
{

#if(BLE_REMOTE_PM_ENABLE)
	if(ui_mic_enable)
	{
		bls_pm_setSuspendMask (MCU_STALL);
	}
	else
	{
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);

		user_task_flg = ota_is_working || scan_pin_need || key_not_released || DEVICE_LED_BUSY;

		if(user_task_flg){
			#if (LONG_PRESS_KEY_POWER_OPTIMIZE)
				extern int key_matrix_same_as_last_cnt;
				if(!ota_is_working && key_matrix_same_as_last_cnt > 5){  //key matrix stable can optize
					bls_pm_setManualLatency(3);
				}
				else{
					bls_pm_setManualLatency(0);  //latency off: 0
				}
			#else
				bls_pm_setManualLatency(0);
			#endif
		}

	#if 1 //deepsleep
		if(sendTerminate_before_enterDeep == 1){ //sending Terminate and wait for ack before enter deepsleep
			if(user_task_flg){  //detect key Press again,  can not enter deep now
				sendTerminate_before_enterDeep = 0;
			}
		}
		else if(sendTerminate_before_enterDeep == 2){  //Terminate OK
			bls_pm_setSuspendMask (DEEPSLEEP_ADV); //when terminate, link layer change back to adc state
			bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0, CONN_DEEP_FLG);
			#if (REMOTE_IR_ENABLE)
				analog_write(DEEP_ANA_REG1, user_key_mode);
			#endif
		}

		//adv 60s, deepsleep
		if( blc_ll_getCurrentState() == BLS_LINK_STATE_ADV && \
			clock_time_exceed(advertise_begin_tick , ADV_IDLE_ENTER_DEEP_TIME * 1000000)){
			bls_pm_setSuspendMask (DEEPSLEEP_ADV); //set deepsleep
			bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0, ADV_DEEP_FLG);
			#if (REMOTE_IR_ENABLE)
				analog_write(DEEP_ANA_REG1, user_key_mode);
			#endif
		}
		//conn 60s no event(key/voice/led), enter deepsleep
		else if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && !user_task_flg && \
				clock_time_exceed(latest_user_event_tick, CONN_IDLE_ENTER_DEEP_TIME * 1000000) ){

			bls_ll_terminateConnection(HCI_ERR_REMOTE_USER_TERM_CONN); //push terminate cmd into ble TX buffer
			sendTerminate_before_enterDeep = 1;
		}
	#endif

	}

#endif  //END of  BLE_REMOTE_PM_ENABLE
}



_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * CLOCK_SYS_CLOCK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_CORE);  //gpio CORE wakeup suspend
	}
}






void user_init()
{
	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value and tp value


////////////////// BLE stack initialization ////////////////////////////////////
	u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};
	u32 *pmac = (u32 *) CFG_ADR_MAC;
	if (*pmac != 0xffffffff)
	{
		memcpy (tbl_mac, pmac, 6);
	}
	else{
		tbl_mac[0] = (u8)rand();
		flash_write_page (CFG_ADR_MAC, 6, tbl_mac);
	}

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU(tbl_mac);   //mandatory

	blc_ll_initAdvertising_module(tbl_mac); 	//adv module: 		 mandatory for BLE slave,
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,
	blc_ll_initPowerManagement_module();        //pm module:      	 optional



	////// Host Initialization  //////////
	extern void my_att_init ();
	my_att_init (); //gatt initialization
	blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization
	bls_smp_enableParing (SMP_PARING_CONN_TRRIGER ); 	//smp initialization


	//HID_service_on_android7p0_init();  //hid device on android 7.0

///////////////////// USER application initialization ///////////////////
	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, \
									 ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_37, ADV_FP_NONE);
	if(status != BLE_SUCCESS){  //adv setting err
		write_reg8(0x8000, 0x11);  //debug
		while(1);
	}
	bls_ll_setAdvEnable(1);  //adv enable
	rf_set_power_level_index (RF_POWER_8dBm);

	//ble event call back
	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &ble_remote_terminate);


	///////////////////// keyboard matrix initialization///////////////////
	u32 pin[] = KB_DRIVE_PINS;
	for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
	{
		gpio_set_wakeup(pin[i],1,1);  	   //drive pin core(gpio) high wakeup suspend
		cpu_set_gpio_wakeup (pin[i],1,1);  //drive pin pad high wakeup deepsleep
	}

#if(KEYSCAN_IRQ_TRIGGER_MODE)
	reg_irq_src = FLD_IRQ_GPIO_EN;
#endif

	bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_keyboard);


	///////////////////// AUDIO initialization///////////////////
#if (BLE_AUDIO_ENABLE)
	//buffer_mic set must before audio_init !!!
	config_mic_buffer ((u32)buffer_mic, TL_MIC_BUFFER_SIZE);

	#if (BLE_DMIC_ENABLE)  //Dmic config
		/////////////// DMIC: PA0-data, PA1-clk, PA3-power ctl
		gpio_set_func(GPIO_PA0, AS_DMIC);
		gpio_set_func(GPIO_PA1, AS_DMIC);

		BM_SET(reg_gpio_config_func0, FLD_DMIC_DI_PWM0);//PA0 as DMIC_DI
		gpio_set_input_en(GPIO_PA0 , 1);                //PA0 as input

		gpio_set_func(GPIO_PA3, AS_GPIO);
		gpio_set_input_en(GPIO_PA3, 1);
		gpio_set_output_en(GPIO_PA3, 1);
		gpio_write(GPIO_PA3, 0);

		#if TL_MIC_32K_FIR_16K
			audio_dmic_init(1, R32, CLOCK_SYS_TYPE);  //1 indicate 1M; 32K
		#else
			audio_dmic_init(1, R64, CLOCK_SYS_TYPE);  //1 indicate 1M; 16K
		#endif
	#else  //Amic config
		//////////////// AMIC: PC3 - bias; PC4/PC5 - input
		#if TL_MIC_32K_FIR_16K
			#if (CLOCK_SYS_CLOCK_HZ == 16000000)
				audio_amic_init( DIFF_MODE, 47,  4, R2, CLOCK_SYS_TYPE);
			#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
				audio_amic_init( DIFF_MODE, 30, 16, R2, CLOCK_SYS_TYPE);
			#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
				audio_amic_init( DIFF_MODE, 65, 15, R2, CLOCK_SYS_TYPE);
			#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
				audio_amic_init( DIFF_MODE, 65, 15, R3, CLOCK_SYS_TYPE);
			#endif
		#else
			#if (CLOCK_SYS_CLOCK_HZ == 16000000)
				audio_amic_init( DIFF_MODE, 65, 15, R2, CLOCK_SYS_TYPE);  // 18 8  34 4
			#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
				audio_amic_init( DIFF_MODE, 65, 15, R3, CLOCK_SYS_TYPE);
			#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
				audio_amic_init( DIFF_MODE, 65, 15, R4, CLOCK_SYS_TYPE);
			#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
				audio_amic_init( DIFF_MODE, 65, 15, R6, CLOCK_SYS_TYPE);
			#endif
		#endif
	audio_finetune_sample_rate(3);//reg0x30[1:0] 2 bits for fine tuning, divider for slow down sample rate
	audio_amic_input_set(PGA_CH);//audio input set, ignore the input parameter
	#endif//end of BLE_DMIC_ENABLE
#endif

#if(BATT_CHECK_ENABLE)
	#if((MCU_CORE_TYPE == MCU_CORE_8261)||(MCU_CORE_TYPE == MCU_CORE_8267)||(MCU_CORE_TYPE == MCU_CORE_8269))
		adc_BatteryCheckInit(ADC_CLK_4M, 1, Battery_Chn_VCC, 0, SINGLEEND, RV_1P428, RES14, S_3);
	#elif(MCU_CORE_TYPE == MCU_CORE_8266)
		adc_Init(ADC_CLK_4M, ADC_CHN_D2, SINGLEEND, ADC_REF_VOL_1V3, ADC_SAMPLING_RES_14BIT, ADC_SAMPLING_CYCLE_6);
	#endif
#endif





		///////////////////// Power Management initialization///////////////////
#if(BLE_REMOTE_PM_ENABLE)
	blc_ll_initPowerManagement_module();
	bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif


	////////////////LED initialization /////////////////////////
	device_led_init(GPIO_LED, 1);

#if (BLE_REMOTE_OTA_ENABLE)
	////////////////// OTA relative ////////////////////////
	bls_ota_clearNewFwDataArea(); //must
	bls_ota_registerStartCmdCb(entry_ota_mode);
	bls_ota_registerResultIndicateCb(LED_show_ota_result);
#endif


#if (REMOTE_IR_ENABLE)
	user_key_mode = analog_read(DEEP_ANA_REG1);
#endif


	//////////////// TEST  /////////////////////////
#if (USER_TEST_BLT_SOFT_TIMER)
	blt_soft_timer_init();
	blt_soft_timer_add(&gpio_test0, 23000);
	blt_soft_timer_add(&gpio_test1, 7000);
	blt_soft_timer_add(&gpio_test2, 13000);
	blt_soft_timer_add(&gpio_test3, 27000);
#endif


	advertise_begin_tick = clock_time();

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
u32 tick_loop;
unsigned short battValue[20];


void main_loop (void)
{
	tick_loop ++;



	#if (USER_TEST_BLT_SOFT_TIMER)
		blt_soft_timer_process(MAINLOOP_ENTRY);
	#endif
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();



	////////////////////////////////////// UI entry /////////////////////////////////
	#if (BLE_AUDIO_ENABLE)
		if(ui_mic_enable){
			task_audio();
		}
	#endif

	#if (BATT_CHECK_ENABLE)
		if(lowBattDet_enable){
			battery_power_check();
		}
	#endif
	//lowBatt_alarmFlag =  //low battery detect

	//proc_keyboard (0,0, 0);

	//device_led_process();

	gpio_set_func(GPIO_PA1, AS_GPIO);
	gpio_set_input_en(GPIO_PA1,0);
	gpio_set_output_en(GPIO_PA1,1);
	while (1) {
		gpio_write(GPIO_PA1,1);
		sleep_us(1000*1000);
		gpio_write(GPIO_PA1,0);
		sleep_us(1000*1000);
	}

	blt_pm_proc();
}


#endif  //end of __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8261_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__
