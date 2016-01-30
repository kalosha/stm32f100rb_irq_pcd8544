#ifndef CIRCULAR_BUFFER_BYTE_H_
#define CIRCULAR_BUFFER_BYTE_H_ 100

#define CIRCULAR_BUFFER_LENGTH 99
#define CIRCULAR_BUFFER_SIZE (CIRCULAR_BUFFER_LENGTH + 1)

typedef struct CIRCULAR_BUFFER_CMD_T {
	uint8_t buffer_cmd;
	uint8_t buffer_data;
} circular_buffer_cmd_t;


typedef struct CIRCULAR_BUFFER_T {
	circular_buffer_cmd_t *buffer;
	uint8_t buffer_size;
	uint8_t buffer_read_index;
	uint8_t buffer_write_index;
} circular_buffer_t;


/*
 * Initialize buffer structure
 * Note, storage space should be (capacity to store) +2
 */
void circular_buffer_init(circular_buffer_t *buf, circular_buffer_cmd_t *buffer, uint8_t size);

/*
 * returns write capacity of the cyclic buffer
 */
uint8_t circular_buffer_write_len(circular_buffer_t *buf);

/*
 * returns read capacity of the cyclic buffer
 */
uint8_t circular_buffer_read_len(circular_buffer_t *buf);


/*
 * Reads one item from cyclic buffer.
 * Returns:
 *  1: if read successful
 *  0: if nothing to read
 */
uint8_t circular_buffer_read(circular_buffer_t *buf, uint8_t *item_cmd, uint8_t *item_data);

/*
 * Writes one item in to cyclic buffer.
 * Returns:
 *  1: if data was written successfully
 *  0: if no space to write
 */
uint8_t circular_buffer_write(circular_buffer_t *buf, uint8_t *item_cmd, uint8_t *item_data);


#endif
