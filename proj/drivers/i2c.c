
#include "../../proj/tl_common.h"
#include "../../proj/mcu/clock.h"
#include "i2c.h"

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL				GPIO_PE4
#endif
#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA				GPIO_PE5
#endif


#define   SDA(a)	gpio_write(PIN_I2C_SDA, a);
#define	  SCL(a)	gpio_write(PIN_I2C_SCL, a);

#define   SDA_IN	 gpio_read(PIN_I2C_SDA)
#define   SCL_IN     gpio_read(PIN_I2C_SCL)

#define   I2C_WAIT  sleep_us(4);

#define WC24C04_1 0xa0
#define RC24C04_1 0Xa1

#define WC24C04_2 0xa2
#define RC24C04_2 0Xa3

void kb_i2c_sim_init()
{
	gpio_write(PIN_I2C_SDA, 1);
	gpio_write(PIN_I2C_SCL, 1);
	gpio_set_func(PIN_I2C_SCL, AS_GPIO);
	gpio_set_func(PIN_I2C_SDA, AS_GPIO);
	gpio_set_output_en(PIN_I2C_SCL, 1);
	gpio_set_output_en(PIN_I2C_SDA, 1);
	gpio_set_input_en(PIN_I2C_SCL, 0);
	gpio_set_input_en(PIN_I2C_SDA, 0);
}

 void  i2c_start(void)
{
	SCL(1);
	I2C_WAIT;
	SDA(1);
	I2C_WAIT;
	SDA(0);
	I2C_WAIT;
}
 void  i2c_stop(void)
{
	SDA(0);
	SCL(0);
	I2C_WAIT;
	SCL(1);
	I2C_WAIT;
	SDA(1);
}

void I2CAck(void)
	{
	gpio_set_output_en(PIN_I2C_SDA, 1);
	gpio_set_input_en(PIN_I2C_SDA, 0);
	SDA(0);
	SCL(0);
	I2C_WAIT;
	SCL(1);
	I2C_WAIT;
	SCL(0);
	}
void I2CNOAck(void)
	{
	gpio_set_output_en(PIN_I2C_SDA, 1);
	gpio_set_input_en(PIN_I2C_SDA, 0);
	SDA(1);
	SCL(0);
	I2C_WAIT;
	SCL(1);
	I2C_WAIT;
	SCL(0);
	}

bool I2CWaitAck(void)   //返回为:1=有ACK,0=无ACK
	{
	SCL(0);

	gpio_set_output_en(PIN_I2C_SDA, 0);
	gpio_set_input_en(PIN_I2C_SDA, 1);

	I2C_WAIT;
	SCL(1);
	I2C_WAIT;
	if(!SDA_IN)
		{
		SCL(0);
		CLOCK_DLY_600NS;
		gpio_set_output_en(PIN_I2C_SDA, 1);
		gpio_set_input_en(PIN_I2C_SDA, 0);
		return true;
		}

	SCL(0);
	CLOCK_DLY_600NS;
	gpio_set_output_en(PIN_I2C_SDA, 1);
	gpio_set_input_en(PIN_I2C_SDA, 0);
	return false;
	}

static void	i2c_write_bit(unsigned char bit)
{
	unsigned char bit_temp;
	bit_temp=!(!bit);
	SCL(0);
	I2C_WAIT;  // this delay is not necessary!
	SDA(bit_temp);
	I2C_WAIT;
	SCL(1);
	I2C_WAIT;

	SCL(0);
	I2C_WAIT;
}

static unsigned char  i2c_read_bit(void)
{
	//i2c_write_bit(1);
	unsigned char rr;

	gpio_set_output_en(PIN_I2C_SDA, 0);
	gpio_set_input_en(PIN_I2C_SDA, 1);

	I2C_WAIT;
	SCL(1);

    rr = !(!SDA_IN);
    I2C_WAIT;
	SCL(0);
	I2C_WAIT;

//	SDA(1);

	return(rr);
}

static unsigned char i2c_write_byte(unsigned char dat)
{
	unsigned char i;
	i = 0x80;
	while(i)
	{
		i2c_write_bit(dat & i);
		i = i >> 1;
	}
	return 1;
	//return  i2c_read_bit();
}

static void i2c_wait_busy(void)
{
	while (SDA_IN == 0);
}

//static unsigned  char   i2c_read_byte(unsigned last)
unsigned  char   i2c_read_byte()
{

	unsigned  char  dat,i;

	gpio_set_output_en(PIN_I2C_SDA, 0);
	gpio_set_input_en(PIN_I2C_SDA, 1);

	dat = 0;
	for(i=0;i<8;i++)
	{
		SCL(0);
	    I2C_WAIT;  // this delay is not necessary!
	    SCL(1);
	    I2C_WAIT;
		if(!SDA_IN)
			dat = dat<<1;
		else
			dat = (dat<<1)|0x01;
	}

	SCL(0);

//	gpio_set_output_en(PIN_I2C_SDA, 1);
//	gpio_set_input_en(PIN_I2C_SDA, 0);

	//i2c_write_bit(last);
	return   dat;
}


bool I2C_ByteWrite(u8* pBuffer, u16 WAddr)
	{
	u8 WC24C04=0;
	u8 WriteAddr=0;
	if(WAddr>255)
		{
		WriteAddr=(u8)(WAddr-255);
		WC24C04= WC24C04_2;
		}   //写第二页
	else
		{
		 WriteAddr=(u8)WAddr;
		 WC24C04= WC24C04_1;
		}//发送器件地址 写  第一页

	i2c_start();//启动I2C
	i2c_write_byte(WC24C04);//发送器件地址 写
	if(I2CWaitAck()==0)return false;
	i2c_write_byte(WriteAddr);
	if(I2CWaitAck() ==0)return false;
	i2c_write_byte(*pBuffer);
	//i2c_write_byte(0x55);
	if(I2CWaitAck()==0) return false;
	i2c_stop();
	return true;
	}

bool I2C_PageWrite(u16* pBuffer, u16 WAddr, u8 n)
{
	u8 i;
	u8 data=0;
	u8 WriteAddr=0;
	u8 WC24C04=0;
	u8 RC24C04=0;

	if(WAddr>255)
		{
		WriteAddr=(u8)(WAddr-256);
		WC24C04= WC24C04_2;
		RC24C04=RC24C04_2;
		}   //写第二页
	else
		{
		 WriteAddr=(u8)WAddr;
		 WC24C04= WC24C04_1;
		 RC24C04=RC24C04_1;
		}//发送器件地址 写  第一页

	i2c_start();//启动I2C
	i2c_write_byte(WC24C04);
	if(I2CWaitAck() == 0)    return false;
	i2c_write_byte(WriteAddr);
	if(I2CWaitAck() == 0)    return false;
	for(i = 0; i < n; i++)//写入8字节数据
		{
		data=(u8)(*pBuffer>>8);
		i2c_write_byte(data);
		if(I2CWaitAck() == 0)return false;
		data=(u8)(*pBuffer);
		i2c_write_byte(data);
		if(I2CWaitAck() == 0)return false;
		pBuffer++;
		}
	i2c_stop();
	sleep_us(10000);
	return true;
}

bool I2C_BufferRead(u16* pBuffer, u16 RAddr, u16 n)
{
	u8 i;
	u8 data0=0;
	u8 data1=0;
	u8	ReadAddr=0;
	u8 WC24C04=0;
	u8 RC24C04=0;

	if(RAddr>255)
		{
		 ReadAddr=(u8)(RAddr-256);
		 WC24C04= WC24C04_2;
		 RC24C04=RC24C04_2;
		}
	else
		{
		 ReadAddr=(u8)RAddr;
		 WC24C04= WC24C04_1;
		 RC24C04=RC24C04_1;
		}

	i2c_start();//启动I2C
	i2c_write_byte(WC24C04);//发送器件地址 写
	if(I2CWaitAck() == 0)    return false;
	i2c_write_byte(ReadAddr);//发送器件内部地址
	if(I2CWaitAck() == 0)return false;
	i2c_start();
	i2c_write_byte(RC24C04); //发送器件地址 读
	if(I2CWaitAck() == 0)return false;

	for(i = 0; i < n - 1; i++)//读取字节数据
	{
		data0=i2c_read_byte();//读取数据
		I2CAck();
		sleep_us(4);
		data1=i2c_read_byte();//读取数据
		I2CAck();
		pBuffer[i]=(u16)(data0<<8)+data1;//读取数据

	}
	data0=i2c_read_byte();//读取数据
	I2CAck();
	sleep_us(4);
	data1=i2c_read_byte();//读取数据
	pBuffer[n - 1] = (u16)(data0<<8)+data1;//读取数据
	I2CNOAck();
	i2c_stop();
	return true;
}

