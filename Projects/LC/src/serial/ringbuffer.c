/**
 * @file ringbuffer.c
 * @brief Implementation of the ring buffer functions.
 * @author Guilherme Matos
 */

#include "ringbuffer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Every variable used by the ring buffer.
*/
struct rb {
  uint8_t *buf;     /// Region of memory where the data is stored
  size_t elem_size; /// Size of each element in the buffer
  size_t size;      /// Size of the buffer
  size_t read_head; /// Index of the next position to be read
  size_t write_head;/// Index of the next position to be written
};

ring_buffer *ring_buffer_new(size_t buf_size, size_t elem_size) {
  if (buf_size == 0)
    buf_size = RING_BUFFER_DEFAULT_SIZE;
  buf_size += elem_size - (buf_size % elem_size);
  ring_buffer *result = malloc(sizeof(ring_buffer));
  if (result == NULL) {
    fprintf(stderr, "Malloc failed: BUY MORE RAM!");
    exit(101);
  }
  result->elem_size = elem_size;
  result->size = buf_size;
  result->buf = calloc(result->size, sizeof(char));
  if (result->buf == NULL) {
    fprintf(stderr, "Calloc failed: BUY MORE RAM!");
    exit(101);
  }
  result->read_head = 0;
  result->write_head = 0;
  return result;
}

void ring_buffer_free(ring_buffer *tgt) {
  free(tgt->buf);
  free(tgt);
  tgt = NULL; // Pointing to NULL to avoid use after free
}

/**
 * @brief Reallocates the buffer to 50% more space.
 * @param rb Pointer to the ring buffer
 */
void ring_buffer_internal_realloc(ring_buffer *rb) {
  size_t old_size = rb->size;
  rb->size = rb->size * 3 / 2;
  rb->size += rb->elem_size - (rb->size % rb->elem_size);
  rb->buf = realloc(rb->buf, rb->size);
  if (rb->buf == NULL) {
    fprintf(stderr, "Realloc failed: BUY MORE RAM!");
    exit(101);
  }
  size_t sz_diff = rb->size - old_size;
  memmove(rb->buf + rb->write_head + sz_diff, rb->buf + rb->write_head,
          old_size - rb->write_head);
  rb->read_head += sz_diff;
}

/**
 * @brief Grows the buffer to 50% more space without moving the data.
 * @param rb Pointer to the ring buffer
 */
void ring_buffer_internal_simple_grow(ring_buffer *rb) {
  rb->size = rb->size * 3 / 2;
  rb->size += rb->elem_size - (rb->size % rb->elem_size);
  rb->buf = realloc(rb->buf, rb->size);
  if (rb->buf == NULL) {
    fprintf(stderr, "Realloc failed: BUY MORE RAM!");
    exit(101);
  }
}

void ring_buffer_push(ring_buffer *rb, void *elem) {
  if (rb->write_head >= rb->read_head && rb->read_head == 0 &&
      (rb->write_head + rb->elem_size) % rb->size <= rb->write_head)
    ring_buffer_internal_simple_grow(rb);
  while ((rb->write_head + rb->elem_size) % rb->size >= rb->read_head &&
         (rb->write_head < rb->read_head))
    ring_buffer_internal_realloc(rb);
  memcpy(rb->buf + rb->write_head, elem, rb->elem_size);
  rb->write_head = (rb->write_head + rb->elem_size) % rb->size;
}

bool ring_buffer_is_empty(ring_buffer *rb) {
  return rb->write_head == rb->read_head;
}

bool ring_buffer_pop(ring_buffer *rb, void *elem) {
  if (ring_buffer_is_empty(rb))
    return false;
  memcpy(elem, rb->buf + rb->read_head, rb->elem_size);
  rb->read_head = (rb->read_head + rb->elem_size) % rb->size;
  return true;
}

void ring_buffer_dump_buffer(ring_buffer *rb) {
  for (size_t i = 0; i < rb->size; ++i) {
    if (i % rb->elem_size == 0) {
      printf("\n");
      if (rb->read_head >= i && rb->read_head < (i + rb->elem_size)) {
        printf(" read> ");
      } else if (rb->write_head >= i && rb->write_head < (i + rb->elem_size)) {
        printf("write> ");
      } else {
        printf("       ");
      }
    }
    printf("%02X ", rb->buf[i]);
  }
  printf("\n");
}
