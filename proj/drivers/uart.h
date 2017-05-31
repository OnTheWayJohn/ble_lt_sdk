/**************************************************************************************************
  Filename:       	uart.h
  Author:			junjun.xu@telink-semi.com
  Created Date:	2016/06/05

  Description:    This file contains the details of enum variables and functions used in the uart.c file


**************************************************************************************************/
#ifndef 	uart_H
#define 	uart_H

#include "../mcu/register.h"
#include "../common/compatibility.h"
#include "../common/utility.h"

/**
 *  @brief  Define parity type
 */
typedef enum {
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD,
} UART_ParityTypeDef;

/**
 *  @brief  Define the length of stop bit
 */
typedef enum {
    STOP_BIT_ONE = 0,
    STOP_BIT_ONE_DOT_FIVE = BIT(12),
    STOP_BIT_TWO = BIT(13),
} UART_StopBitTypeDef;

enum UARTIRQSOURCE{
	UARTRXIRQ,
	UARTTXIRQ,
};

enum{
	UARTRXIRQ_MASK  = BIT(0),
	UARTTXIRQ_MASK  = BIT(1),
	UARTIRQ_MASK    = UARTRXIRQ_MASK | UARTTXIRQ_MASK,
};

/**
 *  @brief  Define UART RTS mode
 */
enum {
    UART_RTS_MODE_AUTO = 0,
    UART_RTS_MODE_MANUAL,
};

#define CLK32M_UART9600         do{\
									uart_Init(237,13,PARITY_NONE,STOP_BIT_ONE);\
									uart_DmaModeInit(1,1);\
								}while(0)
#define CLK32M_UART115200       do{\
									uart_Init(19,13,PARITY_NONE,STOP_BIT_ONE);\
									uart_DmaModeInit(1,1);\
								}while(0)
#define CLK16M_UART115200       do{\
									uart_Init(9,13,PARITY_NONE,STOP_BIT_ONE);\
									uart_DmaModeInit(1,1);\
								}while(0)
#define CLK16M_UART9600         do{\
									uart_Init(103,15,PARITY_NONE,STOP_BIT_ONE);\
									uart_DmaModeInit(1,1);\
								}while(0)

//UART_TX/UART_RX gpio pin config
#define    UART_GPIO_CFG_PA6_PA7()  do{\
										reg_gpio_pa_gpio &= 0x3f;\
										reg_gpio_config_func0 |= 0x80;\
                                    }while(0)
#define    UART_GPIO_CFG_PB2_PB3()  do{\
										reg_gpio_pb_gpio &= 0xf3;\
										reg_gpio_config_func1 |= 0x0c;\
									}while(0)
#define    UART_GPIO_CFG_PC2_PC3()  do{\
										reg_gpio_pc_gpio &= 0xf3;\
										reg_gpio_config_func2 |= 0x0c;\
									}while(0)

#define UART_GPIO_8267_PA6_PA7      1
#define UART_GPIO_8267_PC2_PC3      2
#define UART_GPIO_8267_PB2_PB3      3

#define GET_UART_IRQ_NOT_DMA       ((reg_uart_status0&FLD_UART_IRQ_FLAG) ? 1:0)  //not dma mode,1: occur uart irq; 0:not uart irq
/**********************************************************
*
*	@brief	reset uart module
*
*	@param	none
*
*	@return	none
*/
extern void uart_Reset(void );


/**********************************************************
*
*	@brief	clear error state of uart rx, maybe used when application detected UART not work
*
*	@parm	none
*
*	@return	'1' RX error flag rised and cleard success; '0' RX error flag not rised
*
*/
unsigned char uart_ErrorCLR(void);


/*******************************************************
*
*	@brief	uart initiate, set uart clock divider, bitwidth and the uart work mode
*
*	@param	uartCLKdiv - uart clock divider
*			bwpc - bitwidth, should be set to larger than 2
*			en_rx_irq - '1' enable rx irq; '0' disable.
*			en_tx_irq - enable tx irq; '0' disable.
*
*	@return	'1' set success; '0' set error probably bwpc smaller than 3.
*
*		BaudRate = sclk/((uartCLKdiv+1)*(bwpc+1))
*		SYCLK = 16Mhz
		115200		9			13
		9600		103			15
*
*		SYCLK = 32Mhz
*		115200		19			13
		9600		237			13
*/
extern unsigned char uart_Init(unsigned short uartCLKdiv, unsigned char bwpc, UART_ParityTypeDef Parity,UART_StopBitTypeDef StopBit);
/**
 * @brief     enable uart DMA mode,config uart dam interrupt.
 * @param[in] dmaTxIrqEn -- whether or not enable UART TX interrupt.
 * @param[in] dmaRxIrqEn -- whether or not enable UART RX interrupt.
 * @return    none
 */
extern void uart_DmaModeInit(unsigned char dmaTxIrqEn, unsigned char dmaRxIrqEn);

/**
 * @brief     config the number level setting the irq bit of status register 0x9d
 *            ie 0x9d[3].
 *            If the cnt register value(0x9c[0,3]) larger or equal than the value of 0x99[0,3]
 *            or the cnt register value(0x9c[4,7]) less or equal than the value of 0x99[4,7],
 *            it will set the irq bit of status register 0x9d, ie 0x9d[3]
 * @param[in] rx_level - receive level value. ie 0x99[0,3]
 * @param[in] tx_level - transmit level value.ie 0x99[4,7]
 * @param[in] rx_irq_en - 1:enable rx irq. 0:disable rx irq
 * @param[in] tx_irq_en - 1:enable tx irq. 0:disable tx irq
 * @return    none
 * @notice    suggust closing tx irq.
 */
extern void uart_NotDmaModeInit(unsigned char rx_level,unsigned char tx_level,unsigned char rx_irq_en,unsigned char tx_irq_en);

/********
 * @ brief   in not dma mode, receive the data.
 *           the method to read data should be like this: read receive data in the order from 0x90 to 0x93.
 *           then repeat the order.
 * @ param[in] none
 * @ return    the data received from the uart.
 */
extern unsigned char uart_NotDmaModeRevData(void);

/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    1: send success ; 0: uart busy
 */
extern unsigned char UART_NotDmaModeSendByte(unsigned char uartData);

/********************************************************************************
*	@brief	uart send data function, this  function tell the DMA to get data from the RAM and start
*			the DMA send function
*
*	@param	sendBuff - send data buffer
*
*	@return	'1' send success; '0' DMA busy
*/
extern unsigned char uart_Send(unsigned char* addr);

extern unsigned char uart_Send_kma(unsigned char* addr);
/********************************************************************
*
*	@brief	uart receive function, call this function to get the UART data
*
*	@param	userDataBuff - data buffer to store the uart data
*
*	@return	'0' rx error; 'rxLen' received data length
*/
//extern unsigned short uart_Rec(unsigned char* addr);

/******************************************************************************
*
*	@brief		get the uart IRQ source and clear the IRQ status, need to be called in the irq process function
*
*	@return		uart_irq_src- enum variable of uart IRQ source, 'UARTRXIRQ' or 'UARTTXIRQ'
*
*/
extern enum UARTIRQSOURCE uart_IRQSourceGet(void);

extern enum UARTIRQSOURCE uart_IRQSourceGet_kma(void);
/****************************************************************************************
*
*	@brief	data receive buffer initiate function. DMA would move received uart data to the address space, uart packet length
*			needs to be no larger than (recBuffLen - 4).
*
*	@param	*recAddr:	receive buffer's address info.
*			recBuffLen:	receive buffer's length, the maximum uart packet length should be smaller than (recBuffLen - 4)
*
*	@return	none
*/

extern void uart_RecBuffInit(unsigned char *recAddr, unsigned short recBuffLen);

extern void uart_BuffInit(unsigned char *recAddr, unsigned short recBuffLen, unsigned char *txAddr);

void uart_clr_tx_busy_flag();

void uart_set_tx_done_delay (u32 t);		//for 8266 only

unsigned char uart_tx_is_busy(void);

void uart_io_init(unsigned char uart_io_sel);


#endif
