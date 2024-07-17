/**
 * @file ringbuffer.h
 * @brief Definition of a circular vector. Useful for serial communication.
 * @author Guilherme Matos 
 */

#include <stdbool.h>
#include <stddef.h>

#ifndef RB_GUARD
#define RB_GUARD

#define RING_BUFFER_DEFAULT_SIZE 4096 /// Default size of the buffer in bytes
typedef struct rb ring_buffer;        /// Forward declaration of the ring buffer struct (see ringbuffer.c)

/**
 * @brief Creates a new ring buffer.
 * @param buf_size Size of the buffer
 * @param elem_size Size of each element in the buffer
 * @return A pointer to the newly created and allocated ring buffer
 */
ring_buffer *ring_buffer_new(size_t buf_size, size_t elem_size);

/**
 * @brief Frees the memory allocated for the ring buffer.
 * @param tgt Pointer to the ring buffer to be let go
 */
void ring_buffer_free(ring_buffer *rb);

/**
 * @brief Write an element to the ring buffer.
 * @param rb Pointer to the ring buffer
 * @param elem Pointer to the element to be written
 */
void ring_buffer_push(ring_buffer *rb, void *elem);

/**
 * @brief Read an element from the ring buffer.
 * @param rb Pointer to the ring buffer
 * @param elem Pointer to the element to be read
 * @return True if a element was inside the buffer, false otherwise
 */
bool ring_buffer_pop(ring_buffer *rb, void *elem);

/**
 * @brief Check if the ring buffer is empty, i.e., if the read and write heads are at the same position.
 * @param rb Pointer to the ring buffer
 * @return True if the buffer is empty, false otherwise
 */
bool ring_buffer_is_empty(ring_buffer *rb);

/**
 * @brief Debug function to print the contents of the buffer.
 * @param rb Pointer to the ring buffer
 */
void ring_buffer_dump_buffer(ring_buffer *rb);
#endif
