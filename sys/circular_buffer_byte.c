#include "stm32f10x.h"
#include "circular_buffer_byte.h"

inline uint8_t circular_buffer_write_len(circular_buffer_t *buf) {
	return (buf->buffer_read_index > buf->buffer_write_index) ?
			(buf->buffer_read_index - buf->buffer_write_index - 1) :
			(buf->buffer_size - buf->buffer_write_index + buf->buffer_read_index - 1);
}

inline uint8_t circular_buffer_read_len(circular_buffer_t *buf) {
	return ((buf->buffer_read_index > buf->buffer_write_index) ?
			(buf->buffer_size - buf->buffer_read_index + buf->buffer_write_index - 1) :
			(buf->buffer_write_index - buf->buffer_read_index - 1));
}

void circular_buffer_init(circular_buffer_t *buf, circular_buffer_cmd_t *buffer, uint8_t size) {
	buf->buffer = buffer;
	buf->buffer_size = size;
	buf->buffer_read_index = size - 1;
	buf->buffer_write_index = 0;
}

uint8_t circular_buffer_read(circular_buffer_t *buf, uint8_t *item_cmd, uint8_t *item_data) {
	if (circular_buffer_read_len(buf) > 0) {
		buf->buffer_read_index++;
		if (buf->buffer_read_index >= buf->buffer_size)
			buf->buffer_read_index = 0; // cycle buffer
		*item_cmd = buf->buffer[buf->buffer_read_index].buffer_cmd; // read item at position r
		*item_data = buf->buffer[buf->buffer_read_index].buffer_data; // read item at position r
		return 1;
	}
	return 0; // nothing to read, return 0
}

uint8_t circular_buffer_write(circular_buffer_t *buf, uint8_t *item_cmd, uint8_t *item_data) {
	if (circular_buffer_write_len(buf) > 0) {
		buf->buffer[buf->buffer_write_index].buffer_cmd = *item_cmd; // write item at position r
		buf->buffer[buf->buffer_write_index++].buffer_data = *item_data; // write item at position r
		if (buf->buffer_write_index >= buf->buffer_size)
			buf->buffer_write_index = 0; // cycle buffer
		return 1;
	}
	return 0; // no space to write, return 0
}
