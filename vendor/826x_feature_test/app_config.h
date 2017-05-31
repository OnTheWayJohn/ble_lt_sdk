#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#if  (__PROJECT_8261_FEATURE_TEST__)
	#define CHIP_TYPE				CHIP_TYPE_8261
#elif (__PROJECT_8266_FEATURE_TEST__)
	#define CHIP_TYPE				CHIP_TYPE_8266
#elif (__PROJECT_8267_FEATURE_TEST__)
	#define CHIP_TYPE				CHIP_TYPE_8267
#else
	#define CHIP_TYPE				CHIP_TYPE_8269
#endif




/////////////////// TEST FEATURE SELECTION /////////////////////////////////
#define	TEST_ADVERTISING_ONLY							1
#define TEST_SCANNING_ONLY								2
#define TEST_ADVERTISING_IN_CONN_SLAVE_ROLE				3
#define TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE        4
#define TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE	5

#define TEST_POWER_ADV									10


#define FEATURE_TEST_MODE								TEST_ADVERTISING_IN_CONN_SLAVE_ROLE



#if (   FEATURE_TEST_MODE == TEST_SCANNING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE \
	 || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_SLAVE_ROLE)
	#define BLE_PM_ENABLE								0
#else
	#define BLE_PM_ENABLE								1
#endif

#define USB_ADV_REPORT_TO_PC_ENABLE					1  //display adv report on pc

/////////////////////HCI ACCESS OPTIONS///////////////////////////////////////
#define HCI_NONE		0
#define HCI_USE_UART	1
#define HCI_USE_USB		2
#define HCI_ACCESS		HCI_NONE



/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	16000000
//////////////////Extern Crystal Type///////////////////////
#define CRYSTAL_TYPE			XTAL_12M		//  extern 12M crystal


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms







#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
	#if (__PROJECT_8266_FEATURE_TEST__)
		//ch0-ch7: B0 A5 E5 F0 F1 E7 E6 E4
		#define PB0_INPUT_ENABLE					0
		#define PA5_INPUT_ENABLE					0
		#define PE5_INPUT_ENABLE					0
		#define PF0_INPUT_ENABLE					0
		#define PF1_INPUT_ENABLE					0
		#define PE7_INPUT_ENABLE					0
		#define PE6_INPUT_ENABLE					0
		#define PE4_INPUT_ENABLE					0
		#define PB0_OUTPUT_ENABLE					1
		#define PA5_OUTPUT_ENABLE					1
		#define PE5_OUTPUT_ENABLE					1
		#define PF0_OUTPUT_ENABLE					1
		#define PF1_OUTPUT_ENABLE					1
		#define PE7_OUTPUT_ENABLE					1
		#define PE6_OUTPUT_ENABLE					1
		#define PE4_OUTPUT_ENABLE					1

		#define DBG_CHN0_LOW		( *(unsigned char *)0x80058b &= (~0x01) )   //PB0
		#define DBG_CHN0_HIGH		( *(unsigned char *)0x80058b |= 0x01 )
		#define DBG_CHN0_TOGGLE		( *(unsigned char *)0x80058b ^= 0x01 )
		#define DBG_CHN1_LOW		( *(unsigned char *)0x800583 &= (~0x20) )   //PA5
		#define DBG_CHN1_HIGH		( *(unsigned char *)0x800583 |= 0x20 )
		#define DBG_CHN1_TOGGLE		( *(unsigned char *)0x800583 ^= 0x20 )
		#define DBG_CHN2_LOW		( *(unsigned char *)0x8005a3 &= (~0x20) )   //PE5
		#define DBG_CHN2_HIGH		( *(unsigned char *)0x8005a3 |= 0x20 )
		#define DBG_CHN2_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x20 )
		#define DBG_CHN3_LOW		( *(unsigned char *)0x8005ab &= (~0x01) )   //PF0
		#define DBG_CHN3_HIGH		( *(unsigned char *)0x8005ab |= 0x01 )
		#define DBG_CHN3_TOGGLE		( *(unsigned char *)0x8005ab ^= 0x01 )
		#define DBG_CHN4_LOW		( *(unsigned char *)0x8005ab &= (~0x02) )   //PF1
		#define DBG_CHN4_HIGH		( *(unsigned char *)0x8005ab |= 0x02 )
		#define DBG_CHN4_TOGGLE		( *(unsigned char *)0x8005ab ^= 0x02 )
		#define DBG_CHN5_LOW		( *(unsigned char *)0x8005a3 &= (~0x80) )   //PE7
		#define DBG_CHN5_HIGH		( *(unsigned char *)0x8005a3 |= 0x80 )
		#define DBG_CHN5_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x80 )
		#define DBG_CHN6_LOW		( *(unsigned char *)0x8005a3 &= (~0x40) )   //PE6
		#define DBG_CHN6_HIGH		( *(unsigned char *)0x8005a3 |= 0x40 )
		#define DBG_CHN6_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x40 )
		#define DBG_CHN7_LOW		( *(unsigned char *)0x8005a3 &= (~0x10) )   //PE4
		#define DBG_CHN7_HIGH		( *(unsigned char *)0x8005a3 |= 0x10 )
		#define DBG_CHN7_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x10 )
	#else  //8261/8267/8269
		//ch0-ch7: A7 A4 A3 E0 A1 A0 E1 D3
		#define PA0_INPUT_ENABLE					0
		#define PA1_INPUT_ENABLE					0
		#define PA3_INPUT_ENABLE					0
		#define PA4_INPUT_ENABLE					0
		#define PA7_INPUT_ENABLE					0
		#define PD3_INPUT_ENABLE					0
		#define PE0_INPUT_ENABLE					0
		#define PE1_INPUT_ENABLE					0
		#define PA0_OUTPUT_ENABLE					1
		#define PA1_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PA7_OUTPUT_ENABLE					1
		#define PD3_OUTPUT_ENABLE					1
		#define PE0_OUTPUT_ENABLE					1
		#define PE1_OUTPUT_ENABLE					1

		#define DBG_CHN0_LOW		( *(unsigned char *)0x800583 &= (~0x80) )   //PA7
		#define DBG_CHN0_HIGH		( *(unsigned char *)0x800583 |= 0x80 )
		#define DBG_CHN0_TOGGLE		( *(unsigned char *)0x800583 ^= 0x80 )
		#define DBG_CHN1_LOW		( *(unsigned char *)0x800583 &= (~0x10) )   //PA4
		#define DBG_CHN1_HIGH		( *(unsigned char *)0x800583 |= 0x10 )
		#define DBG_CHN1_TOGGLE		( *(unsigned char *)0x800583 ^= 0x10 )
		#define DBG_CHN2_LOW		( *(unsigned char *)0x800583 &= (~0x08) )   //PA3
		#define DBG_CHN2_HIGH		( *(unsigned char *)0x800583 |= 0x08 )
		#define DBG_CHN2_TOGGLE		( *(unsigned char *)0x800583 ^= 0x08 )
		#define DBG_CHN3_LOW		( *(unsigned char *)0x8005a3 &= (~0x01) )   //PE0
		#define DBG_CHN3_HIGH		( *(unsigned char *)0x8005a3 |= 0x01 )
		#define DBG_CHN3_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x01 )
		#define DBG_CHN4_LOW		( *(unsigned char *)0x800583 &= (~0x02) )   //PA1
		#define DBG_CHN4_HIGH		( *(unsigned char *)0x800583 |= 0x02 )
		#define DBG_CHN4_TOGGLE		( *(unsigned char *)0x800583 ^= 0x02 )
		#define DBG_CHN5_LOW		( *(unsigned char *)0x800583 &= (~0x01) )   //PA0
		#define DBG_CHN5_HIGH		( *(unsigned char *)0x800583 |= 0x01 )
		#define DBG_CHN5_TOGGLE		( *(unsigned char *)0x800583 ^= 0x01 )
		#define DBG_CHN6_LOW		( *(unsigned char *)0x8005a3 &= (~0x02) )   //PE1
		#define DBG_CHN6_HIGH		( *(unsigned char *)0x8005a3 |= 0x02 )
		#define DBG_CHN6_TOGGLE		( *(unsigned char *)0x8005a3 ^= 0x02 )
		#define DBG_CHN7_LOW		( *(unsigned char *)0x80059b &= (~0x08) )   //PD3
		#define DBG_CHN7_HIGH		( *(unsigned char *)0x80059b |= 0x08 )
		#define DBG_CHN7_TOGGLE		( *(unsigned char *)0x80059b ^= 0x08 )
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
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE





/////////////////// PRINT DEBUG INFO ///////////////////////
/* 826x module's pin simulate as a uart tx, Just for debugging */
#define PRINT_DEBUG_INFO                    1//open/close myprintf
#if PRINT_DEBUG_INFO
//defination debug printf pin
#define PRINT_BAUD_RATE             		1000000 //1M baud rate,should Not bigger than 1M, when system clock is 16M.
#if	(__PROJECT_8261_FEATURE_TEST__ || __PROJECT_8267_FEATURE_TEST__ || __PROJECT_8269_FEATURE_TEST__)
#define DEBUG_INFO_TX_PIN           		GPIO_PC6//G0 for 8267/8269 EVK board(C1T80A30_V1.0)
//#define PC6_OUTPUT_ENABLE	        		1       //mini_printf function contain this
#define PULL_WAKEUP_SRC_PC6         		PM_PIN_PULLUP_1M
#else//__PROJECT_8266_FEATURE_TEST__
#define DEBUG_INFO_TX_PIN           		GPIO_PD3//G9 for 8266 EVK board(C1T53A20_V2.0)
//#define PD3_OUTPUT_ENABLE	        		1       //mini_printf function contain this
#define PULL_WAKEUP_SRC_PD3         		PM_PIN_PULLUP_1M
#endif
#endif



#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
