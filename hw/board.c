/*
 * File: board.c
 * Author: George Eliozov
 * Date: 2013-11-12
 */

#include "stm32f10x.h"
#include "board.h"
#include "uart.h"

#ifdef BOARD_UART1_EN
static uint8_t _board_usart1_rx[BOARD_UART_BUF_SIZE];
static uint8_t _board_usart1_tx[BOARD_UART_BUF_SIZE];
static buffer_rx_tx_t _board_usart1_buff;
#endif

#ifdef BOARD_UART2_EN
static uint8_t _board_usart2_rx[BOARD_UART_BUF_SIZE];
static uint8_t _board_usart2_tx[BOARD_UART_BUF_SIZE];
static buffer_rx_tx_t _board_usart2_buff;
#endif


// ##### general.start ####

void board_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
//	direct set
//	GPIOx->BSRR = GPIO_Pin;
	GPIO_SetBits(GPIOx, GPIO_Pin);
}

void board_ClearBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
//	direct reset
//	GPIOx->BRR = GPIO_Pin;
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}

void board_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
//	direct toggle
//	GPIOx->ODR ^= GPIO_Pin;

	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	GPIOx->ODR ^= GPIO_Pin;
}

// ##### general.end ####



void board_init_uart() {
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

#ifdef BOARD_UART1_EN
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	// Init USART1

	// GPIO
	GPIO_StructInit(&GPIO_InitStruct);
	// init TX
	GPIO_InitStruct.GPIO_Pin = BOARD_UART1_TX_PIN; // TX pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// init RX
	GPIO_StructInit(&GPIO_InitStruct); // kalo
	GPIO_InitStruct.GPIO_Pin = BOARD_UART1_RX_PIN;  // RX pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	buf_init(&(_board_usart1_buff.rx), _board_usart1_rx, BOARD_UART_BUF_SIZE);
	buf_init(&(_board_usart1_buff.tx), _board_usart1_tx, BOARD_UART_BUF_SIZE);
	uart_init(USART1, BOARD_UART1_SPEED, &_board_usart1_buff);
	/* Enable the USARTx Interrupt */

	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif

#ifdef BOARD_UART2_EN
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// Init USART2
	gpioInit.GPIO_Pin = GPIO_Pin_2; // TX pin
	gpioInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpioInit);
	gpioInit.GPIO_Pin = GPIO_Pin_3; // RX pin
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioInit);

	buf_init(&(_board_usart2_buff.rx), _board_usart2_rx, BOARD_UART_BUF_SIZE);
	buf_init(&(_board_usart2_buff.tx), _board_usart2_tx, BOARD_UART_BUF_SIZE);
	uart_init(USART2, BOARD_UART2_SPEED, &_board_usart2_buff);
	/* Enable the USARTx Interrupt */

	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif

}

void board_led_init() {
#ifdef BOARD_LEDS_EN

	GPIO_InitTypeDef gpioInit;
	RCC_APB2PeriphClockCmd(BOARD_LED_PERIPH, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = BOARD_LED_BLUE_PIN;
	GPIO_Init(BOARD_LED_BLUE_GPIO, &gpioInit);
	gpioInit.GPIO_Pin = BOARD_LED_GREEN_PIN;
	GPIO_Init(BOARD_LED_GREEN_GPIO, &gpioInit);
#endif
}

#ifdef BOARD_LEDS_EN
void board_led_on(board_leds_e leds) {
	if (b_led_blue & leds)
		board_SetBit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN);
	if (b_led_green & leds)
		board_SetBit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN);
}

void board_led_off(board_leds_e leds) {
	if (b_led_blue & leds)
		board_ClearBit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN);
	if (b_led_green & leds)
		board_ClearBit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN);
}

void board_led_toggle(board_leds_e leds){
	if (b_led_blue & leds)
		board_ToggleBit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN);
	if (b_led_green & leds)
		board_ToggleBit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN);
}
#endif

void board_init() {

    RCC_DeInit ();                    /* RCC system reset(for debug purpose)*/
    RCC_HSEConfig (RCC_HSE_ON);       /* Enable HSE                         */

    /* Wait till HSE is ready                                               */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                */
    RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                  */
    RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                */
    RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4               */

    /* PLLCLK = 8MHz * 3 = 24 MHz                                           */
    RCC_PLLConfig (0x00010000, RCC_PLLMul_3);

    RCC_PLLCmd (ENABLE);                  /* Enable PLL                     */

    /* Wait till PLL is ready                                               */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source                                    */
    RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source                         */
    while (RCC_GetSYSCLKSource() != 0x08);

    /* Enable USART1 and GPIOA clock                                        */
//    RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);


	board_init_uart();
	board_led_init();
}

void board_delay_ms(uint32_t ms) {
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);

  nCount = (RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}

void board_delay_sec(uint32_t sec) {
	board_delay_ms(sec*1000);
}
