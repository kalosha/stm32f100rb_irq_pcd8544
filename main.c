/*
 *
 */

#include "stm32F10x.h"

#include "hw/board.h"
#include "hw/pcd8544/pcd8544.h"

#define LCD_TEST

void cmd_background(char *s);

static uint32_t _delay_counter = 0;
void delay_ms(uint32_t delay_value) {
	_delay_counter = delay_value;
	while (_delay_counter)
		;
}
void SysTick_Handler(void) {
	if (_delay_counter != 0) {
		_delay_counter--;
	}
}


int main(void) {
	if (SysTick_Config(SystemCoreClock / 1000))
		while (1) ;

	board_init();
	cmd_init("\n\nPCD8544 v0.1 @kalo.devel.ge\n");
	term_register_cmd("bg", cmd_background, "Set background PWM value {bg 0|1000}");
	PCD8544_init_LED();

	board_led_on(b_led_green);
#ifdef PCD8544_H
	//Initialize LCD with 0x38 software contrast

	PCD8544_Init(0x38);
	board_led_on(b_led_blue);

	//Go to x=14, y=3 position
	PCD8544_GotoXY(14, 3);

	PCD8544_SetContrast(0x39);
	//Print data with Pixel Set mode and Fontsize of 5x7px
	PCD8544_Puts("STM32F100RB", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(15, 13);
	PCD8544_Puts("Discovery", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(30, 26);
	PCD8544_Puts("2016", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);

	PCD8544_GotoXY(45, 42);
	//Put string with Pixel set mode and Fontsize of 3x5
	PCD8544_Puts("devel.ge", PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

	//Display data on LCD
	PCD8544_Refresh();
	board_delay_ms(1000);

//	TIM_SetCompare2 (TIM2 , 500);
#endif


#ifdef LCD_TEST
	uint8_t _char = 0x00;
#endif

//	PCD8544_DrawLine(0,0,1,4,PCD8544_Pixel_Set);
//	PCD8544_DrawLine(0,0,20,20,PCD8544_Pixel_Clear);
	while (1) {
//		PCD8544_RST_HIGH;
//		board_delay_ms(100);
//		board_delay_ms(1);
		board_led_toggle(b_led_blue);
#ifdef LCD_TEST
		PCD8544_Write(PCD8544_DATA,_char++);
#endif
		cmd_mthread_proc();
		PCD8544_Refresh();
	}
}


void cmd_background(char *s){
	int result;
	int args[1];
	result = prs_args_i(&s,args,1,',');
	if(result>0){
		PCD8544_setBackground(args[0]);
		xprintf("background #%d\n", args[0]);
		return;
	}
}






//
//    Process the interrupts for SPI1.
//
void SPI1_IRQHandler() {
	PCD8544_SPI_IRQHandler();
}


