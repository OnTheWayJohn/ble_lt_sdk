
#include "../../proj/tl_common.h"

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble/ll/ll.h"

#if (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8266_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)


#define PIN_I2C_INT				GPIO_PC4
//unsigned char byte1 = 0x00;
//unsigned char byte2 = 0x00;
//unsigned char byte3 = 0x00;
//char run_here = 0;
u16* pPage;
bool read_I2C_ok;
u8 john_debug,i;

bool i2c_ready=false;
u8 spi_WIP=1;

extern void user_init();
extern void main_loop (void);
extern void deep_wakeup_proc(void);

_attribute_ram_code_ void irq_handler(void)
{
#if (BLE_IR_ENABLE)
	u32 src = reg_irq_src;
	if(src & FLD_IRQ_TMR1_EN){
		ir_irq_send();
		reg_tmr_sta = FLD_TMR_STA_TMR1;
	}

	if(src & FLD_IRQ_TMR2_EN){
		ir_repeat_handle();
		reg_tmr_sta = FLD_TMR_STA_TMR2;
	}
#endif

	irq_blt_sdk_handler ();

}





int main (void) {

	cpu_wakeup_init();

	set_tick_per_us (CLOCK_SYS_CLOCK_HZ/1000000);  //1us=1000000
	clock_init();

	gpio_init();


	gpio_set_func(GPIO_PF0, AS_SPI); //SO
	gpio_set_input_en(GPIO_PF0,0);
	gpio_set_output_en(GPIO_PF0,1);

	gpio_set_func(GPIO_PF1, AS_SPI);   //sck
	gpio_set_input_en(GPIO_PF1,0);
	gpio_set_output_en(GPIO_PF1,1);

	gpio_set_func(GPIO_PE7, AS_SPI);   //SI
	gpio_set_input_en(GPIO_PE7,1);
	gpio_set_output_en(GPIO_PE7,0);

	gpio_set_func(GPIO_PE6, AS_SPI);   //cs
	gpio_set_input_en(GPIO_PE6,0);
	gpio_set_output_en(GPIO_PE6,1); //for SPI

//	gpio_write(GPIO_PC4, 1);
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 0);
	gpio_set_input_en(GPIO_PC4,1);

	gpio_write(GPIO_PC7, 1);
	gpio_set_func(GPIO_PC7, AS_GPIO);
	gpio_set_output_en(GPIO_PC7, 1);
	gpio_set_input_en(GPIO_PC7,0);

	spi_init(16,0);					//for SPI
	kb_i2c_sim_init();
//	sleep_us(10000) ;

//SPI initial command
//		spi_write(data,ncs), data 1 byte, ncs: chip select 0 or 1;
		spi_write(0x06,1);  //spi_write(data,ncs)



// Flash Sector Erase
		spi_write(0x20,0);  //command erase sector
		spi_write(00,0); //spi_write(address23-16,ncs)
		spi_write(0,0);  //spi_write(address15-7,ncs)
		spi_write(0,1);  //spi_write(address7-0,ncs)

		gpio_write(GPIO_PC7, spi_WIP);   //debug pin to check the WIP status
		while (spi_WIP)
		{
		  spi_write(0x05,0);  //command erase sector
		  john_debug=spi_read(1);
		  if(john_debug & 0x01) spi_WIP=1;
		  else spi_WIP=0;
		  gpio_write(GPIO_PC7, spi_WIP);  //debug pin to check the WIP status
		}


//SPI write 1 byte data to a 24 bit address

		spi_write(0x06,1);  //spi_write(data,ncs)

		spi_write(0x02,0);  //spi_write(data,ncs)   //command datasheet
		spi_write(00,0);    //spi_write(data,ncs)     //address bit23-bit16
		spi_write(00,0);    //spi_write(data,ncs)     //address bit15-bit8
		spi_write(0,0);     //spi_write(data,ncs)     //address bit15-bit8
		for (i=0;i<254;i++)
		   {
			spi_write(i,0);  //spi_write(data,ncs)      //address bit7-bit0

		   }
			spi_write(0xee,1);
		sleep_us(1000) ;

//I2C initial data
//		for (i=0;i<256;i++)    //write data to EEPROM
//		{
	  //I2C_ByteWrite(u8 pBuffer, u16 WAddr)
//		I2C_ByteWrite(i,i);
//		sleep_us(1000);
//		}


	while (1)
	{

   //     while (~PIN_I2C_INT)
    //    {

//        }

//I2C application
//I2C write data byt
   	   //I2C_ByteWrite(u8 pBuffer, u16 WAddr)
//		 I2C_ByteWrite(0x48,0xAF);
//		 I2C_ByteWrite(0x48,0x5c);
//		 I2C_PageWrite(0xAF48,0x5c,2);
//    	gpio_write(GPIO_PC7, PIN_I2C_INT);

//Use interrupt function to detect the GPIO_PC4.
		i2c_ready=gpio_read(GPIO_PC4);
		i2c_ready=true;
//I2C read data bytes
		if(i2c_ready)
		{
//			read_I2C_ok=I2C_BufferRead(&pPage, 0xAF48, 4);  //"4" read 8 byte data/



//SPI read 3 byte data from 24 bits address;
//		spi_write(data,ncs), data 1 byte, ncs: chip select 0 or 1;
		spi_write(0x03,0);  //spi_write(data,ncs)
		spi_write(00,0);    //spi_write(address23-16,ncs)
		spi_write(00,0);    //spi_write(address15-7,ncs)
		spi_write(0,0);     //spi_write(address7-0,ncs)

		for (i=0;i<254;i++)
		 {
		   john_debug=spi_read(0);   //spi_read(ncs);
		 }
		 john_debug=spi_read(1);   //spi_read(ncs);

		sleep_us(100);

//SPI write 1 byte data to a 24 bit address
/*		spi_write(0x02,0);  //spi_write(data,ncs)   //command datasheet
		spi_write(23,0);  //spi_write(data,ncs)     //address bit23-bit16
		spi_write(15,0);  //spi_write(data,ncs)     //address bit15-bit8
		spi_write(7,0);  //spi_write(data,ncs)      //address bit7-bit0
		spi_write(0x35,1);
*/
		sleep_us(100);
		i2c_ready=false;
		}



	}

	while (1)
	{
		sleep_us(100);
	}
	deep_wakeup_proc();

	rf_drv_init(CRYSTAL_TYPE);

	user_init ();

    irq_enable();

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}
}



#endif
