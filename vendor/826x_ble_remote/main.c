
#include "../../proj/tl_common.h"

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble/ll/ll.h"

#if (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8266_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)

unsigned char byte1 = 0x00;
unsigned char byte2 = 0x00;
unsigned char byte3 = 0x00;
char run_here = 0;

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

#define WC24C04_1 0xa0
#define RC24C04_1 0Xa1

#define WC24C04_2 0xa2
#define RC24C04_2 0Xa3



char john_debug = 0;
char john_debug2 = 0;
int main (void) {
	char init_value = 0;
	char sda_value = 0;
	char clk_value = 0;
	unsigned  char  dat,i,x;
	cpu_wakeup_init();

	set_tick_per_us (CLOCK_SYS_CLOCK_HZ/1000000);
	clock_init();

	gpio_init();

/*
	gpio_set_func(GPIO_PC2, AS_GPIO);
	gpio_set_input_en(GPIO_PC2,0);
	gpio_set_output_en(GPIO_PC2,1);

	gpio_set_func(GPIO_PC6, AS_GPIO);
	gpio_set_input_en(GPIO_PC6,0);
	gpio_set_output_en(GPIO_PC6,1);

	gpio_set_func(GPIO_PC7, AS_GPIO);
	gpio_set_input_en(GPIO_PC7,0);
	gpio_set_output_en(GPIO_PC7,1);
	i2c_write_bit(1);
	while (1) {
		gpio_write (GPIO_PC7,1);
		i2c_write_byte(0x5C);
		i2c_write_byte(0xAF);
		i2c_write_byte(0x48);
		gpio_write (GPIO_PC7,0);
		sleep_us(1000);

	}
*/
	/*gpio_set_func(GPIO_PC2, AS_GPIO);
	gpio_set_input_en(GPIO_PC2,1);
	gpio_set_output_en(GPIO_PC2,0);

	gpio_set_func(GPIO_PC6, AS_GPIO);
	gpio_set_input_en(GPIO_PC6,1);
	gpio_set_output_en(GPIO_PC6,0);

	gpio_set_func(GPIO_PC7, AS_GPIO);
	gpio_set_input_en(GPIO_PC7,1);
	gpio_set_output_en(GPIO_PC7,0);

	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_input_en(GPIO_PC4,0);
	gpio_set_output_en(GPIO_PC4,1);

	gpio_set_func(GPIO_PC1, AS_GPIO);
	gpio_set_input_en(GPIO_PC1,0);
	gpio_set_output_en(GPIO_PC1,1);

	gpio_set_func(GPIO_PA1, AS_GPIO);
	gpio_set_input_en(GPIO_PA1,0);
	gpio_set_output_en(GPIO_PA1,1);*/
/*
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
	gpio_set_output_en(GPIO_PE6,1);*/ //for SPI

	//spi_init(16,0);					//for SPI

	kb_i2c_sim_init();
	I2C_ByteWrite(0x44, 0x00);
	I2C_ByteWrite(0x55, 0x01);
	I2C_ByteWrite(0x66, 0x02);
	while (1)
	{
		/*init_value = gpio_read(PIN_INIT);
		gpio_write(GPIO_PC4, init_value);

		sda_value = gpio_read(GPIO_PC2);
		gpio_write(GPIO_PC1, sda_value);

		clk_value = gpio_read(GPIO_PC6);
		gpio_write(GPIO_PA1, clk_value);*/

		//sleep_us(50);
		/*if (init_value == 1) {
			gpio_write(GPIO_PC4, 0);
			byte1 = i2c_read_byte();
			run_here = 3;
			//byte2 = i2c_read_byte();
			run_here = 4;
			//byte3 = i2c_read_byte();
			//If there is no data coming next, should break;
		}*/
		//spi_slave_init(0,NULL);
		//spi_write(0x0F);
		//spi_write(0x01);
		//spi_write(0x13);
		//spi_write(0x00);
		//spi_write(0x00);
		//spi_write(0x00);
		//sleep_us(1);
		//spi_write(0x0F);
		//spi_write(0x00);
		//spi_write(0x05);
		//john_debug=spi_read();
		//spi_write(0x35);
		//john_debug2=spi_read();
		//reg_spi_data = 0x85;
		//john_debug = reg_spi_data;
		//sleep_us(10);
	//	spi_write(0x59);
		//I2C_ByteWrite(0xA0,0);
		//i2c_write_byte(0x00);
		//i2c_write_byte(0xA1);
		u16 read_data =0;
		sda_value = I2C_BufferRead(&read_data, 0, 1);
		sda_value = I2C_BufferRead(&read_data, 1, 1);
		sda_value = I2C_BufferRead(&read_data, 2, 1);
		sleep_us(100);
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
