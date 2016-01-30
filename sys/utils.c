/*
 * utils.c
 *
 *  Created on: 29.08.2010
 *      Author: nerozero
 */
#include "stm32f10x.h"
#include <stdarg.h>
#include <math.h>

#include "utils.h"


int xatof(char **str, double *res)
{
    // Parse optional leading minus sign.
	int result = 0;
    double sign = 1.0;
    double d = 0.0;
    double sf;
    if (**str == '-') {
        sign = -1.0;
        (*str)++;
    }

	while (**str == ' ')(*str)++; // trim left

    // Parse any digits before decimal point.
    while (isdigit(**str)) {
        d *= 10.0;
        d += *(*str)++ - '0';
        result = 1;
    }
    // Parse optional decimal point.
    if (**str == '.') {
    	*(*str)++;
        // Parse any digits after decimal point.
        sf = 0.1;
        while (isdigit(**str)) {
            d += sf * (*(*str)++ - '0');
            sf *= 0.1;
            result = 1;
        }
    }
    *res = sign * d;
    return result;
}

int xatoi (char **str, long *res)
{
	uint32_t val;
	uint8_t c, radix, s = 0;
	int result = 0;


	while ((c = **str) == ' ') (*str)++;
	if (c == '-') {
		s = 1;
		c = *(++(*str));
	}
	if (c == '0')
	{
		c = *(++(*str));
		if (c <= ' '|| c == ',')//||c ==';')
		{
			*res = 0; return 1;
		}
		if (c == 'x') {
			radix = 16;
			c = *(++(*str));
		} else {
			if (c == 'b') {
				radix = 2;
				c = *(++(*str));
			} else {
				if ((c >= '0')&&(c <= '9'))
				{
					radix = 8;
				}else{
					return 1; // value is zero, corrected by nerozero
				}

			}
		}
	} else {
		if ((c < '1')||(c > '9'))
			return 0;
		radix = 10;
	}
	val = 0;
	while (c > ' ') {
		if (c >= 'a') c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			//if (c <= 9) return 0;
			if (c <= 9) break; // read only number, break after
		}
		//if (c >= radix) return 0;
		if (c >= radix) break;
		result = 1;
		val = val * radix + c;
		c = *(++(*str));
	}
	if (s) val = -val;
	*res = val;
	return result;
}

void xputc (char c)
{
	if (c == '\n') UTIL_PUTC_USART1('\r');
	UTIL_PUTC_USART1(c);
}

void xputs (const char* str)
{
	while (*str)
		xputc(*str++);
}

void xputs_raw(const char* str)
{
	while (*str)
		UTIL_PUTC_USART1(*str++);
}




void xitoa (long val, int radix, int len)
{
	uint8_t c, r, sgn = 0, pad = ' ';
	uint8_t s[20], i = 0;
	uint32_t v;


	if (radix < 0) {
		radix = -radix;
		if (val < 0) {
			val = -val;
			sgn = '-';
		}
	}
	v = val;
	r = radix;
	if (len < 0) {
		len = -len;
		pad = '0';
	}
	if (len > 20) return;
	do {
		c = (uint8_t)(v % r);
		if (c >= 10) c += 7;
		c += '0';
		s[i++] = c;
		v /= r;
	} while (v);
	if (sgn) s[i++] = sgn;
	while (i < len)
		s[i++] = pad;
	do
		xputc(s[--i]);
	while (i);
}


char *xftoa(double num, char *str,uint8_t prec)
{
	char *s;
	short l,v;
	l = v = 0;
	s = str;
	if(num<0)
	{
		*s++='-';
		num = fabs(num);
	}
	while(num>=10)
	{
		l++;
		num/=10;
	}
	while(l>=0 || prec)
	{
		v = (uint8_t)num;
		num -= v;
		num*=10;
		*s++ = v+0x30;
		if(l<0) prec--;
		if(l==0) *s++ = '.';
		l--;
	}
	*s=0;
	return s;
}

void xputf(double num, uint8_t prec,xcb_putchar putcharp)
{
	char buf[20];
	char *pbuf;
	short l,l2,v,cf;
	if (prec == 0) prec = 3;
	if (prec > 10) prec = 10;


	// conversation
	l = v = 0;
	pbuf = buf;
	if(num<0)
	{
		putcharp('-');
		num = fabs(num);
	}
	while(num>=10)
	{
		l++;
		num/=10;
	}
	l2 =l;
	prec+=1;
	while(l2>=0 || prec)
	{
		v = (uint8_t)num;
		num -= v;
		num*=10;
		*pbuf++ =v;
		if(l2<0) prec--;
		l2--;
	}
	*pbuf-- = 0xff; // the end;
#ifdef U_ENABLE_DOUBLE_ROUND
	cf = 0;
	if(*pbuf>=5)
	{
		cf=1;

	}
	*pbuf = 0;

	while(--pbuf>=buf)
	{
		if(cf)
		{
			(*pbuf)++;
			cf =0;
		}
		if(*pbuf>9)
		{
			*pbuf=0;
			cf = 1;
		}
	}
#endif
	pbuf = buf;
	l2 =l;
	while(*pbuf<=9)
	{
		putcharp(*pbuf++ + 0x30);
		if(l2==0) putcharp('.');
		l2--;
		if(*(pbuf+1)>9) break;
	}
	/*
	while(l>=0 || prec)
	{
		v = (uint8_t)num;
		num -= v;
		num*=10;
		xputc(v+0x30);
		if(l<0) prec--;
		if(l==0) xputc('.');
		l--;
	}
	*/
}

void xprintf (const char* str, ...)
{
	va_list arp;
	int d, r, w, s, l;
	double f;


	va_start(arp, str);

	while ((d = *str++) != 0) {
		if (d != '%') {
			xputc(d); continue;
		}
		d = *str++; w = r = s = l = 0;
		if (d == '0') {
			d = *str++; s = 1;
		}
		while ((d >= '0')&&(d <= '9')) {
			w += w * 10 + (d - '0');
			d = *str++;
		}
		if (s) w = -w;

		if (d == 'l') {
			l = 1;
			d = *str++;
		}
		if (!d) break;
		if(d == '%'){
			xputc(d);
			continue;
		}
		if (d == 's') {
			xputs(va_arg(arp, char*));
			continue;
		}
		if(d == 'f'){
			f = (double )va_arg(arp, double);
			xputf(f,w,xputc);
			continue;
		}
		if (d == 'c') {
			xputc((char)va_arg(arp, int));
			continue;
		}
		if (d == 'u') r = 10;
		if (d == 'd') r = -10;
		if ((d == 'X') || (d == 'x')) r = 16;
		if (d == 'b') r = 2;
		if (!r) break;
		if (l) {
			xitoa((long)va_arg(arp, long), r, w);
		} else {
			if (r > 0)
				xitoa((unsigned long)va_arg(arp, int), r, w);
			else
				xitoa((long)va_arg(arp, int), r, w);
		}
	}

	va_end(arp);
}




void put_dump (const uint8_t *buff, uint32_t ofs, int cnt)
{
	uint8_t n;


	xprintf("%08lX ", ofs);
	for(n = 0; n < cnt; n++)
		xprintf(" %02X", buff[n]);
	xputc(' ');
	for(n = 0; n < cnt; n++) {
		if ((buff[n] < 0x20)||(buff[n] >= 0x7F))
			xputc('.');
		else
			xputc(buff[n]);
	}
	xputc('\n');
}

char *inttostr(char *c, int32_t val, uint8_t dp)
{
	uint32_t a;
	uint8_t id = 0, m=0;
	char buf[20];
	char *pbuf;
	pbuf = buf;
	if(val<0)
	{
		m=1;
		val=-val;
	}

	a = val;
	while(a || id<= dp)
	{
		id++;
		*pbuf++='0'+a%10;
		if(id==dp)*pbuf++='.';
		a=a/10;
	}
	if(m)
		*c++ ='-';
	else
		*c++ = ' ';
	while(pbuf>buf)
		*c++ = *--pbuf;
	*c =0;
	return c;
}





/*

void get_line (char *buff, int len)
{
	char c;
	int idx = 0;


	for (;;) {
		c = xgetc();
		if (c == '\r') break;
		if ((c == '\b') && idx) {
			idx--; xputc(c); xputc(' '); xputc(c);
		}
		if (((uint8_t)c >= ' ') && (idx < len - 1)) {
				buff[idx++] = c; xputc(c);
		}
	}
	buff[idx] = 0;
	xputc('\n');
}

*/
