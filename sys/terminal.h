/*
 * File: terminal.h
 * Author: George Eliozov
 * Date: 2013-11-26
 */

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "parser.h"
#include "hw/uart.h"

#define TERM_PUTC_USART1(c)		(uart_putchar(USART1,c))
#define TERM_GETC_USART1()			(uart_getchar(USART1))

#define TERM_MAX_COMMANDS		20
#define TERM_BUF_LEN			80

typedef int (*term_rxhook_t)(char c);

void term_init();
void term_process_received();
void term_put_cmd_prefix();
int term_register_cmd(char *name, prs_cmd_callback_t callback, char *help);


#endif
