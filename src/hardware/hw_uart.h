

#pragma once

#include "../platform.h"
#include "../common/fifo.h"


typedef struct {
  fifo_s* fifoRx;
  fifo_s* fifoTx;    
  uint32_t boudrate;  
} hw_uart_cfg_s;


//----------------------------------------------------------------------------------------
// UART1
#define DEBUG_USARTx           USART1
#define DEBUG_USART_CLK        RCC_APB2_PERIPH_USART1
#define DEBUG_USART_APBxClkCmd RCC_APB2_Peripheral_Clock_Enable
#define DEBUG_USART_BAUDRATE   115200

// Macro definition of USART GPIO pin
#define DEBUG_USART_GPIO_CLK        RCC_AHB_PERIPH_GPIOA
#define DEBUG_USART_GPIO_APBxClkCmd RCC_AHB_Peripheral_Clock_Enable

#define DEBUG_USART_TX_GPIO_PORT GPIOA
#define DEBUG_USART_TX_GPIO_PIN  GPIO_PIN_9
#define DEBUG_USART_RX_GPIO_PORT GPIOA
#define DEBUG_USART_RX_GPIO_PIN  GPIO_PIN_10

#define DEBUG_USART_Tx_GPIO_AF GPIO_AF5_USART1
#define DEBUG_USART_Rx_GPIO_AF GPIO_AF5_USART1

#define DEBUG_USART_IRQ        USART1_IRQn
#define DEBUG_USART_IRQHandler USART1_IRQHandler

extern hw_uart_cfg_s hwDebugUart;

#define MY_UART_IS_RX_INT       UART_IsRxReadyInt(MY_UART)
#define MY_UART_IS_RX_READY     UART_IsRxReady(MY_UART)
#define MY_UART_IS_TX_INT       UART_IsTHREInt(MY_UART)
#define MY_UART_READ            UART_Read(MY_UART)
#define MY_UART_WRITE(a)        UART_Write(MY_UART, a)
#define MY_UART_DISABLE_TX_INT  USART_Interrput_Disable(DEBUG_USARTx, USART_INT_TXDE)
#define MY_UART_ENABLE_TX_INT   USART_Interrput_Enable(DEBUG_USARTx, USART_INT_TXDE)

void hw_debug_uart_init(void);

//----------------------------------------------------------------------------------------
// ¹âÁ÷
// UART3
#define OPFLOW_USARTx           UART3
#define OPFLOW_USART_CLK        RCC_APB2_PERIPH_UART3
#define OPFLOW_USART_APBxClkCmd RCC_APB2_Peripheral_Clock_Enable
#define OPFLOW_USART_BAUDRATE   19200

// Macro definition of USART GPIO pin
#define OPFLOW_USART_GPIO_CLK        RCC_AHB_PERIPH_GPIOB
#define OPFLOW_USART_GPIO_APBxClkCmd RCC_AHB_Peripheral_Clock_Enable


#if (_VERSION_ == VERSION_3)     
#define OPFLOW_USART_TX_GPIO_PORT GPIOB
#define OPFLOW_USART_TX_GPIO_PIN  GPIO_PIN_4
#define OPFLOW_USART_RX_GPIO_PORT GPIOB
#define OPFLOW_USART_RX_GPIO_PIN  GPIO_PIN_5

#define OPFLOW_USART_Tx_GPIO_AF GPIO_AF7_UART3
#define OPFLOW_USART_Rx_GPIO_AF GPIO_AF8_UART3
#elif (_VERSION_ == VERSION_4) 
#define OPFLOW_USART_TX_GPIO_PORT GPIOB
#define OPFLOW_USART_TX_GPIO_PIN  GPIO_PIN_10
#define OPFLOW_USART_RX_GPIO_PORT GPIOB
#define OPFLOW_USART_RX_GPIO_PIN  GPIO_PIN_11

#define OPFLOW_USART_Tx_GPIO_AF GPIO_AF10_UART3
#define OPFLOW_USART_Rx_GPIO_AF GPIO_AF10_UART3
#endif

#define OPFLOW_USART_IRQ        UART3_IRQn
#define OPFLOW_USART_IRQHandler UART3_IRQHandler


extern hw_uart_cfg_s hwOpflowUart;

void hw_opticalflow_uart_init(void);




