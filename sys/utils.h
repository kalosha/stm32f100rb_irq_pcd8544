/*
 * utils.h
 *
 *  Created on: 29.08.2010
 *      Author: nerozero
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "../hw/uart.h"

// Enable rounding of 1.59999999 -> 1.60000000
#define U_ENABLE_DOUBLE_ROUND

#define UTIL_PUTC_USART1(c) 	uart_putchar(USART1,c)

#define _BV(value)			(1<<value)

typedef  void (*xcb_putchar)(char);


int xatof(char **s, double *res);
int xatoi (char**, long*);
void xputc (char);
void xputs (const char*);
void xputs_raw(const char*);
void xitoa (long, int, int);
void xprintf (const char*, ...);
void xputf(double num, uint8_t prec,xcb_putchar putcharp);
double xround(double num, uint8_t prec);
void put_dump (const uint8_t*, uint32_t ofs, int cnt);
char *inttostr(char *c, int32_t val, uint8_t dp);
char *xftoa(double num, char *str,uint8_t prec);



#endif /* UTILS_H_ */
