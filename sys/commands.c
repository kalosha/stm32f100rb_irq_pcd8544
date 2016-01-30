/*
 * File: commands.c
 * Author: George Eliozov
 * Date: 2013-11-26
 */

#include "stm32f10x.h"
#include "terminal.h"
#include "utils.h"
#include "commands.h"
#include "../hw/board.h"

#define CMD_U2_BUF_SIZE		80

void cmd_mthread_proc() {
	term_process_received();
}

void cmd_green(char *s){
	int result;
	int args[1];
	result = prs_args_i(&s,args,1,',');
#ifdef BOARD_LEDS_EN
	if(result>0){
		if (args[0]){
			board_led_on(b_led_green);
		} else {
			board_led_off(b_led_green);
		}
		xprintf("LED GREEN#%d\n",1);
		return;
	}
#endif
	xprintf("LED GREEN#%d\n",0);
}

//void cmd_test(char *s) {
//	xprintf("Text received: %s\n", s);
//}

void cmd_init(char *welcome) {
	term_init(welcome);
//	term_register_cmd("test", cmd_test);
	term_register_cmd("led_green", cmd_green, "Green Led on/of {led_green 1|0}");
}
