/*
 * buffer.c
 *
 *  Created on: Aug 22, 2013
 *      Author: nerozero
 */
#include "stm32f10x.h"
#include "buffer.h"


void buf_init(buffer_t *buf, uint8_t *storage, uint8_t size)
{
	buf->storage = storage;
	buf->size = size;
	buf->r = size-1;
	buf->w = 0;
}

inline
uint8_t buf_wr_len(buffer_t *buf)
{
	 return (buf->r > buf->w) ?
			 (buf->r - buf->w -1) :
			 (buf->size - buf->w+buf->r -1);
}

inline
uint8_t buf_rd_len(buffer_t *buf)
{
	return ( (buf->r > buf->w) ?
			(buf->size-buf->r+buf->w-1) :
			(buf->w-buf->r-1) );
}



uint8_t buf_read(buffer_t *buf, uint8_t *item)
{
	if(buf_rd_len(buf)>0)
	{
		buf->r++;
		if(buf->r >= buf->size)
			buf->r = 0; // cycle buffer
		*item = buf->storage[buf->r]; // read item at position r
		return 1;
	}
	return 0; // nothing to read, return 0
}

uint8_t buf_write(buffer_t *buf, uint8_t *item)
{
	if(buf_wr_len(buf)>0)
	{
		buf->storage[buf->w++] = *item; // write item at position r
		if(buf->w >= buf->size )
			buf->w = 0; // cycle buffer
		return 1;
	}
	return 0; // no space to write, return 0
}
