
#ifndef __BOARD_H__
#define __BOARD_H__

#include "../sys/common.h"

#define BOARD_UART_BUF_SIZE			20
#define BOARD_UART1_EN
//#define BOARD_UART2_EN
//#define BOARD_SPI_EN
#define BOARD_LEDS_EN





#ifdef BOARD_UART1_EN
//	#define BOARD_UART1_SPEED			9600
//	#define BOARD_UART1_SPEED			57600
	#define BOARD_UART1_SPEED			115200

	#define BOARD_UART1_TX_PIN			GPIO_Pin_9
	#define BOARD_UART1_RX_PIN			GPIO_Pin_10

#endif

#ifdef BOARD_UART2_EN
	#define BOARD_UART2_SPEED			9600
//	#define BOARD_UART2_SPEED			57600
//	#define BOARD_UART2_SPEED			115200

	#define BOARD_UART2_TX_PIN			GPIO_Pin_2
	#define BOARD_UART2_RX_PIN			GPIO_Pin_3

#endif


#ifdef BOARD_LEDS_EN

	#define BOARD_LED_PERIPH					RCC_APB2Periph_GPIOC
	#define BOARD_LED_BLUE_GPIO					GPIOC
	#define BOARD_LED_BLUE_PIN					GPIO_Pin_8
	#define BOARD_LED_GREEN_GPIO				GPIOC
	#define BOARD_LED_GREEN_PIN					GPIO_Pin_9

	typedef enum{
		b_led_blue = 0x01,
		b_led_green = 0x02,
	}board_leds_e;

	void board_led_on(board_leds_e leds);
	void board_led_off(board_leds_e leds);
	void board_led_toggle(board_leds_e leds);

#endif



void board_init();

void board_delay_ms(uint32_t ms);

void board_delay_sec(uint32_t sec);

void board_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void board_ClearBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void board_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
