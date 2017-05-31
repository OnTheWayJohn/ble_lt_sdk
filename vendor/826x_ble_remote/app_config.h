#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif



#if (__PROJECT_8261_BLE_REMOTE__)
	#define CHIP_TYPE				CHIP_TYPE_8261
#elif (__PROJECT_8266_BLE_REMOTE__)
	#define CHIP_TYPE				CHIP_TYPE_8266
#elif (__PROJECT_8267_BLE_REMOTE__)
	#define CHIP_TYPE				CHIP_TYPE_8267
#elif (__PROJECT_8269_BLE_REMOTE__)
	#define CHIP_TYPE				CHIP_TYPE_8269
#else

#endif



/////////////////// MODULE /////////////////////////////////
#define BLE_REMOTE_PM_ENABLE			0
#define BLE_REMOTE_SECURITY_ENABLE      1
#define BLE_REMOTE_OTA_ENABLE			1
#define REMOTE_IR_ENABLE				0
#define BATT_CHECK_ENABLE       		0   //enable or disable battery voltage detection

#if (__PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)
	#define BLE_AUDIO_ENABLE				1
#else   //8261/8266  not support audio
	#define BLE_AUDIO_ENABLE				0
#endif



////////////////////////// AUDIO CONFIG /////////////////////////////
#if (BLE_AUDIO_ENABLE)
	#define BLE_DMIC_ENABLE					0  //0: Amic   1: Dmic
	#define	ADPCM_PACKET_LEN				128
	#define TL_MIC_ADPCM_UNIT_SIZE			248

	#define	TL_MIC_32K_FIR_16K				1

	#if TL_MIC_32K_FIR_16K
		#define	TL_MIC_BUFFER_SIZE				1984
	#else
		#define	TL_MIC_BUFFER_SIZE				992
	#endif

	#define GPIO_AMIC_BIAS					GPIO_PC6

#endif

///////////// avoid ADC module current leakage (when module on suspend status) //////////////////////////////
#define ADC_MODULE_CLOSED               BM_CLR(reg_adc_mod, FLD_ADC_CLK_EN)  // adc clk disable
#define ADC_MODULE_ENABLE               BM_SET(reg_adc_mod, FLD_ADC_CLK_EN) // adc clk open

//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	1   //proc fast scan when deepsleep back trigged by key press, in case key loss
#define KEYSCAN_IRQ_TRIGGER_MODE		1
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1
#define KB_MAP_REPEAT					{VK_1, }


#define			CR_VOL_UP		0xf0
#define			CR_VOL_DN		0xf1
#define			CR_VOL_MUTE		0xf2
#define			CR_POWER		0xf3
#define			CR_SEL			0xf4
#define			CR_UP			0xf5
#define			CR_DN			0xf6
#define			CR_LEFT			0xf7
#define			CR_RIGHT		0xf8
#define			CR_HOME			0xf9
#define			CR_REWIND		0xfa
#define			CR_NEXT			0xfb
#define			CR_PREV			0xfc
#define			CR_STOP			0xfd


#define		 	VOICE					0xc0
#define 		VOICE_KEYCODE_OUT		0x40
#define 		KEY_MODE_SWITCH			0xc1


#define 		IR_VK_0			0x00
#define 		IR_VK_1			0x01
#define 		IR_VK_2			0x02
#define			IR_VK_3			0x03
#define			IR_VK_4			0x04
#define 		IR_VK_5			0x05
#define 		IR_VK_6			0x06
#define 		IR_VK_7			0x07
#define 		IR_VK_8			0x08
#define 		IR_VK_9			0x09

#define 		IR_POWER		0x12
#define			IR_AUDIO_MUTE	0x0d
#define 		IR_NETFLIX		0x0f
#define			IR_BACK			0x0e
#define			IR_VOL_UP		0x0b
#define			IR_VOL_DN		0x0c
#define 		IR_NEXT			0x20
#define 		IR_PREV			0x21
#define			IR_MENU			0x23
#define 		IR_HOME			0x24
#define 		IR_OPER_KEY		0x2e
#define 		IR_INFO			0x2f
#define			IR_REWIND		0x32
#define 		IR_FAST_FOWARD	0x34
#define 		IR_PLAY_PAUSE	0x35
#define			IR_GUIDE		0x41
#define 		IR_UP			0x45
#define			IR_DN			0x44
#define 		IR_LEFT			0x42
#define 		IR_RIGHT		0x43
#define			IR_SEL			0x46
#define 		IR_RED_KEY		0x6b
#define 		IR_GREEN_KEY	0x6c
#define 		IR_YELLOW_KEY	0x6d
#define 		IR_BLUE_KEY		0x6e
#define 		IR_RECORD		0x72
#define 		IR_OPTION		0x73
#define 		IR_STOP			0x74
#define 		IR_SEARCH		0x75
#define 		IR_TEXT			0x76
#define 		IR_VOICE		0x77
#define 		IR_PAUSE		0x78

#define			T_VK_CH_UP		0xd0
#define			T_VK_CH_DN		0xd1


//8261/8267/8269 hardware: C59T80A5_V1.1
#if (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)

	#define			GPIO_LED				GPIO_PC7

	#if (BLE_IR_ENABLE)  //with IR keymap
			#define 		GPIO_IR_CONTROL			GPIO_PD0

			#define		KB_MAP_NORMAL	{\
							{0, 	1,		2,		3,		4,		5,		6,		}, \
							{7,		8,		9,		10,		VOICE,	12,		13,		}, \
							{14,	15,		16,		17,		18,		19, 	KEY_MODE_SWITCH, }, \
							{21,	22,		23,		24,		25,		26,		27,		}, \
							{28,	29,		30,		31,		32,		33,		34,		}, \
							{35,	36,		37,		38,		39,		40,		41,		}, \
							{42,	43,		44,		45,		46,		47,		48,		}, }

			#define		KB_MAP_BLE	{\
							VK_7,		VK_4,			VK_1,		VK_NONE,			VK_VOL_DN,	VK_VOL_UP,	VK_ESC	, \
							VK_LEFT,	VK_NONE,		VK_NONE,	VK_FAST_BACKWARD,	VK_NONE,	VK_NONE,	VK_POWER, \
							VK_RIGHT,	VK_HOME,		VK_NONE,	VK_FAST_FORWARD,	VK_STOP,	VK_NONE,	KEY_MODE_SWITCH , \
							VK_9,	 	VK_6,			VK_3,		VK_NONE,			VK_CH_DN,	VK_CH_UP,	VK_APP , \
							VK_NONE,	VK_W_MUTE,		VK_NONE,	VK_DOWN,			VK_ENTER,	VK_UP,		VK_NONE , \
							VK_NONE,	VK_NONE,		VK_0,		VK_8,				VK_5,		VK_2,		VK_NONE , \
							VK_W_STOP,	VK_PLAY_PAUSE,	VK_NONE,	VK_NONE,			VK_NONE,	VK_NONE,	VK_NONE, }


			#define		KB_MAP_IR	{\
							IR_VK_7,	IR_VK_4,	IR_VK_1,	IR_RED_KEY,	IR_VOL_DN,	IR_VOL_UP,	IR_BACK	, \
							IR_LEFT,	VK_NONE,	VK_NONE,	IR_PREV,	VK_NONE,	VK_MMODE,	IR_POWER, \
							IR_RIGHT,	IR_HOME,	VK_NONE,	IR_NEXT,	IR_STOP,	IR_SEARCH,	KEY_MODE_SWITCH , \
							IR_VK_9,	IR_VK_6,	IR_VK_3,	IR_BLUE_KEY,VK_CH_DN,	VK_CH_UP,	IR_MENU , \
							IR_YELLOW_KEY,VK_NONE,	VK_NONE,	IR_DN,		IR_SEL,		IR_UP,		VK_NONE , \
							VK_NONE,	VK_NONE,	IR_VK_0,	IR_VK_8,	IR_VK_5,	IR_VK_2,	IR_GREEN_KEY , \
							IR_PAUSE,	VK_NONE,	VK_NONE,	VK_NONE,	VK_NONE,	VK_NONE,	VK_NONE, }
	#else   //key map

			#define		KB_MAP_NORMAL	{\
							VK_7,		VK_4,			VK_1,		VK_NONE,			VK_VOL_DN,	VK_VOL_UP,	VK_ESC	, \
							VK_LEFT,	VK_NONE,		VK_NONE,	VK_FAST_BACKWARD,	VOICE,		VK_NONE,	VK_POWER, \
							VK_RIGHT,	VK_HOME,		VK_NONE,	VK_FAST_FORWARD,	VK_STOP,	VK_NONE,	KEY_MODE_SWITCH , \
							VK_9,	 	VK_6,			VK_3,		VK_NONE,			VK_CH_DN,	VK_CH_UP,	VK_APP , \
							VK_NONE,	VK_W_MUTE,		VK_NONE,	VK_DOWN,			VK_ENTER,	VK_UP,		VK_NONE , \
							VK_NONE,	VK_NONE,		VK_0,		VK_8,				VK_5,		VK_2,		VK_NONE , \
							VK_W_STOP,	VK_PLAY_PAUSE,	VK_NONE,	VK_NONE,			VK_NONE,	VK_NONE,	VK_NONE, }

	#endif  //end of REMOTE_IR_ENABLE


	#define  KB_DRIVE_PINS  {GPIO_PA6, GPIO_PA7, GPIO_PB1, GPIO_PB4, GPIO_PB5, GPIO_PB6, GPIO_PB7}
	#define  KB_SCAN_PINS   {GPIO_PD3, GPIO_PD4, GPIO_PD5, GPIO_PD6, GPIO_PD7, GPIO_PE0, GPIO_PE1}

	#define	PA6_FUNC				AS_GPIO
	#define	PA7_FUNC				AS_GPIO
	#define	PB1_FUNC				AS_GPIO
	#define	PB4_FUNC				AS_GPIO
	#define	PB5_FUNC				AS_GPIO
	#define	PB6_FUNC				AS_GPIO
	#define	PB7_FUNC				AS_GPIO
	//drive pin need 100K pulldown
	#define	PULL_WAKEUP_SRC_PA6		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PA7		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PB1		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PB4		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PB5		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PB6		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PB7		MATRIX_ROW_PULL
	//drive pin open input to read gpio wakeup level
	#define PA6_INPUT_ENABLE		1
	#define PA7_INPUT_ENABLE		1
	#define PB1_INPUT_ENABLE		1
	#define PB4_INPUT_ENABLE		1
	#define PB5_INPUT_ENABLE		1
	#define PB6_INPUT_ENABLE		1
	#define PB7_INPUT_ENABLE		1


	#define	PD3_FUNC				AS_GPIO
	#define	PD4_FUNC				AS_GPIO
	#define	PD5_FUNC				AS_GPIO
	#define	PD6_FUNC				AS_GPIO
	#define	PD7_FUNC				AS_GPIO
	#define	PE0_FUNC				AS_GPIO
	#define	PE1_FUNC				AS_GPIO
	//scan  pin need 10K pullup
	#define	PULL_WAKEUP_SRC_PD3		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PD4		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PD5		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PD6		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PD7		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PE0		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PE1		MATRIX_COL_PULL
	//scan pin open input to read gpio level
	#define PD3_INPUT_ENABLE		1
	#define PD4_INPUT_ENABLE		1
	#define PD5_INPUT_ENABLE		1
	#define PD6_INPUT_ENABLE		1
	#define PD7_INPUT_ENABLE		1
	#define PE0_INPUT_ENABLE		1
	#define PE1_INPUT_ENABLE		1


#else  //8266 hardware: C43T53A5_V1.0

	#define	GPIO_LED							GPIO_PC2

#define		KB_MAP_NORMAL	{\
					{VK_NONE,		VK_3,	  	VK_1,		VK_NONE,	}, \
					{VK_2,	 		VK_5,	  	VK_NONE,	VK_4,	 	}, \
					{VK_NONE,	 	VK_NONE,	VK_NONE,	VK_NONE,	}, \
					{VK_NONE,	 	VK_NONE,	VK_NONE,	VK_NONE,	}, \
					{VK_VOL_UP,	 	VK_NONE,	VK_NONE,	VK_VOL_DN,	}, \
					{VK_NONE,		VK_NONE,	VK_NONE,	VK_NONE,	}, }

	#define  KB_DRIVE_PINS  {GPIO_PA1, GPIO_PA5, GPIO_PA6, GPIO_PA7}
	#define  KB_SCAN_PINS   {GPIO_PB7, GPIO_PC6, GPIO_PE5, GPIO_PE6, GPIO_PF0, GPIO_PE4}

	//drive pin need 100K pulldown
	#define	PULL_WAKEUP_SRC_PA1		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PA5		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PA6		MATRIX_ROW_PULL
	#define	PULL_WAKEUP_SRC_PA7		MATRIX_ROW_PULL
	//scan  pin need 10K pullup
	#define	PULL_WAKEUP_SRC_PB7		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PC6		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PE5		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PE6		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PF0		MATRIX_COL_PULL
	#define	PULL_WAKEUP_SRC_PE4		MATRIX_COL_PULL

	//drive pin open input to read gpio wakeup level
	#define PA1_INPUT_ENABLE		1
	#define PA5_INPUT_ENABLE		1
	#define PA6_INPUT_ENABLE		1
	#define PA7_INPUT_ENABLE		1

	//scan pin open input to read gpio level
	#define PB7_INPUT_ENABLE		1
	#define PC6_INPUT_ENABLE		1
	#define PE5_INPUT_ENABLE		1
	#define PE6_INPUT_ENABLE		1
	#define PF0_INPUT_ENABLE		1
	#define PE4_INPUT_ENABLE		1

#endif





#define		KB_MAP_NUM		KB_MAP_NORMAL
#define		KB_MAP_FN		KB_MAP_NORMAL


/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	16000000

//////////////////Extern Crystal Type///////////////////////
#define CRYSTAL_TYPE			XTAL_12M		//  extern 12M crystal


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms









//////////////////////// DEBUG GPIO ///////////////////////////
#define  DEBUG_GPIO_ENABLE					0

#if(DEBUG_GPIO_ENABLE)

	//8261/8267/8269 hardware: C59T80A5_V1.1
	#if (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)
			#define PA2_FUNC				AS_GPIO //debug gpio chn0 : A2
			#define PA3_FUNC				AS_GPIO //debug gpio chn1 : A3
			#define PA4_FUNC				AS_GPIO //debug gpio chn2 : A4
			#define PA5_FUNC				AS_GPIO //debug gpio chn3 : A5
			#define PB2_FUNC				AS_GPIO //debug gpio chn4 : B2
			#define PB3_FUNC				AS_GPIO //debug gpio chn5 : B3

			#define PA2_OUTPUT_ENABLE					1
			#define PA3_OUTPUT_ENABLE					1
			#define PA4_OUTPUT_ENABLE					1
			#define PA5_OUTPUT_ENABLE					1
			#define PB2_OUTPUT_ENABLE					1
			#define PB3_OUTPUT_ENABLE					1

			#define DBG_CHN0_LOW		( *(unsigned char *)0x800583 &= (~0x04) )
			#define DBG_CHN0_HIGH		( *(unsigned char *)0x800583 |= 0x04 )
			#define DBG_CHN0_TOGGLE		( *(unsigned char *)0x800583 ^= 0x04 )
			#define DBG_CHN1_LOW		( *(unsigned char *)0x800583 &= (~0x08) )
			#define DBG_CHN1_HIGH		( *(unsigned char *)0x800583 |= 0x08 )
			#define DBG_CHN1_TOGGLE		( *(unsigned char *)0x800583 ^= 0x08 )
			#define DBG_CHN2_LOW		( *(unsigned char *)0x800583 &= (~0x10) )
			#define DBG_CHN2_HIGH		( *(unsigned char *)0x800583 |= 0x10 )
			#define DBG_CHN2_TOGGLE		( *(unsigned char *)0x800583 ^= 0x10 )
			#define DBG_CHN3_LOW		( *(unsigned char *)0x800583 &= (~0x20) )
			#define DBG_CHN3_HIGH		( *(unsigned char *)0x800583 |= 0x20 )
			#define DBG_CHN3_TOGGLE		( *(unsigned char *)0x800583 ^= 0x20 )
			#define DBG_CHN4_LOW		( *(unsigned char *)0x80058b &= (~0x04) )
			#define DBG_CHN4_HIGH		( *(unsigned char *)0x80058b |= 0x04 )
			#define DBG_CHN4_TOGGLE		( *(unsigned char *)0x80058b ^= 0x04 )
			#define DBG_CHN5_LOW		( *(unsigned char *)0x80058b &= (~0x08) )
			#define DBG_CHN5_HIGH		( *(unsigned char *)0x80058b |= 0x08 )
			#define DBG_CHN5_TOGGLE		( *(unsigned char *)0x80058b ^= 0x08 )
	#else //8266 hardware: C43T53A5_V1.0
			#define PB0_FUNC				AS_GPIO //debug gpio chn0 : PB0
			#define PC3_FUNC				AS_GPIO //debug gpio chn1 : PC3
			#define PE7_FUNC				AS_GPIO //debug gpio chn2 : PE7
			#define PF1_FUNC				AS_GPIO //debug gpio chn3 : PF1

			#define PB0_INPUT_ENABLE					0
			#define PC3_INPUT_ENABLE					0
			#define PE7_INPUT_ENABLE					0
			#define PF1_INPUT_ENABLE					0
			#define PB0_OUTPUT_ENABLE					1
			#define PC3_OUTPUT_ENABLE					1
			#define PE7_OUTPUT_ENABLE					1
			#define PF1_OUTPUT_ENABLE					1

			#define DBG_CHN0_LOW		( *(unsigned char *)0x80058b &= (~0x01) )
			#define DBG_CHN0_HIGH		( *(unsigned char *)0x80058b |= 0x01 )
			#define DBG_CHN0_TOGGLE		( *(unsigned char *)0x80058b ^= 0x01 )
			#define DBG_CHN1_LOW		( *(unsigned char *)0x800593 &= (~0x08) )
			#define DBG_CHN1_HIGH		( *(unsigned char *)0x800593 |= 0x08 )
			#define DBG_CHN1_TOGGLE		( *(unsigned char *)0x800593 ^= 0x08 )
			#define DBG_CHN2_LOW		( *(unsigned char *)0x8005a3 &= (~0x80) )
			#define DBG_CHN2_HIGH		( *(unsigned char *)0x8005a3 |= 0x80 )
			#define DBG_CHN2_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x80 )
			#define DBG_CHN3_LOW		( *(unsigned char *)0x8005ab &= (~0x02) )
			#define DBG_CHN3_HIGH		( *(unsigned char *)0x8005ab |= 0x02 )
			#define DBG_CHN3_TOGGLE		( *(unsigned char *)0x8005ab ^= 0x02 )
	#endif


#else
#define DBG_CHN0_LOW
#define DBG_CHN0_HIGH
#define DBG_CHN0_TOGGLE
#define DBG_CHN1_LOW
#define DBG_CHN1_HIGH
#define DBG_CHN1_TOGGLE
#define DBG_CHN2_LOW
#define DBG_CHN2_HIGH
#define DBG_CHN2_TOGGLE
#define DBG_CHN3_LOW
#define DBG_CHN3_HIGH
#define DBG_CHN3_TOGGLE
#define DBG_CHN4_LOW
#define DBG_CHN4_HIGH
#define DBG_CHN4_TOGGLE
#define DBG_CHN5_LOW
#define DBG_CHN5_HIGH
#define DBG_CHN5_TOGGLE

#endif  //end of DEBUG_GPIO_ENABLE



/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
