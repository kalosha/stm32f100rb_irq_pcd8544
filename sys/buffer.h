/*
 * buffer.h
 *
 *  Created on: Aug 22, 2013
 *      Author: nerozero
 */

#ifndef BUFFER_H_
#define BUFFER_H_


typedef struct BUFFER_T {
	uint8_t *storage;
	uint8_t size;
	uint8_t r;
	uint8_t w;
} buffer_t;


/*
 * returns write capacity of the cyclic buffer
 */
uint8_t buf_wr_len(buffer_t *buf);

/*
 * returns read capacity of the cyclic buffer
 */
uint8_t buf_rd_len(buffer_t *buf);

/*
 * Initialize buffer structure
 * Note, storage space should be (capacity to store) +2
 */
void buf_init(buffer_t *buf, uint8_t *storage, uint8_t size);

/*
 * Reads one item from cyclic buffer.
 * Returns:
 *  1: if read successful
 *  0: if nothing to read
 */
uint8_t buf_read(buffer_t *buf, uint8_t *item);

/*
 * Writes one item in to cyclic buffer.
 * Returns:
 *  1: if data was written successfully
 *  0: if no space to write
 */
uint8_t buf_write(buffer_t *buf, uint8_t *item);

#endif /* BUFFER_H_ */
