
#include "hw_uart.h"


hw_uart_cfg_s hwDebugUart, hwOpflowUart;


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void DEBUG_USART_IRQHandler(void)
{    
  uint16_t t = hwDebugUart.fifoTx->tail;
	uint16_t h = hwDebugUart.fifoRx->head;
	uint8_t c;
	  
	if(USART_Flag_Status_Get(DEBUG_USARTx, USART_FLAG_RXDNE) != RESET) {   
		USART_Flag_Clear(DEBUG_USARTx, USART_FLAG_RXDNE);  
    
//		while(MY_UART_IS_RX_READY) {  //Get all the input characters		  
		  c = USART_Data_Receive(DEBUG_USARTx);;
      if (++h >= hwDebugUart.fifoRx->length) h = 0;
      if (h != hwDebugUart.fifoRx->tail) {
        hwDebugUart.fifoRx->buf[hwDebugUart.fifoRx->head] = c;
        hwDebugUart.fifoRx->head = h;
	    }
//	  }
  }

  // Is TxFIFO Empty Interrupt?
  if(USART_Flag_Status_Get(DEBUG_USARTx, USART_FLAG_TXDE) != RESET) {    
    if (t != hwDebugUart.fifoTx->head) { 
      if (++t >= hwDebugUart.fifoTx->length) t = 0;
      USART_Data_Send(DEBUG_USARTx, hwDebugUart.fifoTx->buf[hwDebugUart.fifoTx->tail]);  // Transmit next byte in the ring
      hwDebugUart.fifoTx->tail = t;
    }
		
    if (t == hwDebugUart.fifoTx->head)              // Check if all data is transmitted . if yes disable transmitter UDRE interrupt
    {
			USART_Interrput_Disable(DEBUG_USARTx, USART_INT_TXDE);
    }
  }	
}


void hw_debug_uart_init(void)
{  
    //----------------------------------------------------------------
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    // Turn on the clock of usart port GPIO
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK);               

    // Turn on the clock of usart peripheral
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK);
  
    GPIO_Structure_Initialize(&GPIO_InitStructure);
  
    // Configure GPIO of USART TX as push pull multiplexing mode
    GPIO_InitStructure.Pin        = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = DEBUG_USART_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // Configure GPIO of USART RX as floating input mode
    GPIO_InitStructure.Pin        = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Alternate = DEBUG_USART_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    // Configure parameters of usart port
    // Configure baud rate
    USART_InitStructure.BaudRate = hwDebugUart.boudrate;
    // Configure the length of frame data
    USART_InitStructure.WordLength = USART_WL_8B;
    // Configure stop bits
    USART_InitStructure.StopBits = USART_STPB_1;
    // Configure check bit
    USART_InitStructure.Parity = USART_PE_NO;
    // Configure hardware flow control
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    // Configure working mode, send and receive together
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    // Complete initialization configuration of usart port
    USART_Initializes(DEBUG_USARTx, &USART_InitStructure);

    // Configuration interrupt priority of the usart port    
    NVIC_InitType NVIC_InitStructure;
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = DEBUG_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    // Enable usart port receive interrupt    
    USART_Interrput_Enable(DEBUG_USARTx, USART_INT_RXDNE);

    // Enable usart
    USART_Enable(DEBUG_USARTx);
}


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void OPFLOW_USART_IRQHandler(void)
{    
  uint16_t t = hwOpflowUart.fifoTx->tail;
	uint16_t h = hwOpflowUart.fifoRx->head;
	uint8_t c;
	  
	if(USART_Flag_Status_Get(OPFLOW_USARTx, USART_FLAG_RXDNE) != RESET) {   
		USART_Flag_Clear(OPFLOW_USARTx, USART_FLAG_RXDNE);  
    
//		while(MY_UART_IS_RX_READY) {  //Get all the input characters		  
		  c = USART_Data_Receive(OPFLOW_USARTx);;
      if (++h >= hwOpflowUart.fifoRx->length) h = 0;
      if (h != hwOpflowUart.fifoRx->tail) {
        hwOpflowUart.fifoRx->buf[hwOpflowUart.fifoRx->head] = c;
        hwOpflowUart.fifoRx->head = h;
	    }
//	  }
  }

  // Is TxFIFO Empty Interrupt?
  if(USART_Flag_Status_Get(OPFLOW_USARTx, USART_FLAG_TXDE) != RESET) {    
    if (t != hwOpflowUart.fifoTx->head) { 
      if (++t >= hwOpflowUart.fifoTx->length) t = 0;
      USART_Data_Send(OPFLOW_USARTx, hwOpflowUart.fifoTx->buf[hwOpflowUart.fifoTx->tail]);  // Transmit next byte in the ring
      hwOpflowUart.fifoTx->tail = t;
    }
		
    if (t == hwOpflowUart.fifoTx->head)              // Check if all data is transmitted . if yes disable transmitter UDRE interrupt
    {
			USART_Interrput_Disable(OPFLOW_USARTx, USART_INT_TXDE);
    }
  }	
}

void hw_opticalflow_uart_init(void)
{  
    //----------------------------------------------------------------
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    // Turn on the clock of usart port GPIO
    OPFLOW_USART_GPIO_APBxClkCmd(OPFLOW_USART_GPIO_CLK);               

    // Turn on the clock of usart peripheral
    OPFLOW_USART_APBxClkCmd(OPFLOW_USART_CLK);
  
    GPIO_Structure_Initialize(&GPIO_InitStructure);
  
    // Configure GPIO of USART TX as push pull multiplexing mode
    GPIO_InitStructure.Pin            = OPFLOW_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = OPFLOW_USART_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(OPFLOW_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // Configure GPIO of USART RX as floating input mode
    GPIO_InitStructure.Pin            = OPFLOW_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Alternate = OPFLOW_USART_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(OPFLOW_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    // Configure parameters of usart port
    // Configure baud rate
    USART_InitStructure.BaudRate = hwOpflowUart.boudrate;
    // Configure the length of frame data
    USART_InitStructure.WordLength = USART_WL_8B;
    // Configure stop bits
    USART_InitStructure.StopBits = USART_STPB_1;
    // Configure check bit
    USART_InitStructure.Parity = USART_PE_NO;
    // Configure hardware flow control
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    // Configure working mode, send and receive together
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    // Complete initialization configuration of usart port
    USART_Initializes(OPFLOW_USARTx, &USART_InitStructure);

    // Configuration interrupt priority of the usart port    
    NVIC_InitType NVIC_InitStructure;
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = OPFLOW_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    // Enable usart port receive interrupt    
    USART_Interrput_Enable(OPFLOW_USARTx, USART_INT_RXDNE);

    // Enable usart
    USART_Enable(OPFLOW_USARTx);
}




