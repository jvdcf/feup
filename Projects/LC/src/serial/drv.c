/**
 * @file drv.c
 * @brief Implementation of the serial port driver.
 * @author Guilherme Matos
*/

#include "drv.h"
#include "minix/syslib.h"
#include "minix/sysutil.h"
#include "ringbuffer.h"
#include <lcom/lcf.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static ring_buffer *IN_RING = NULL;
static ring_buffer *OUT_RING = NULL;
static int32_t BIT_NO;
static bool TCV_READY = false;
static bool RCV_READY = false;

void _RECV_READY_FN() {
  RCV_READY = true;
}

void _OVERRUN_ERR() {
  fprintf(stderr, "ERROR: OVERRUN");
}

void _PARITY_ERR() {
  fprintf(stderr, "ERROR: PARITY");
}

void _FRAMING_ERR() {
  fprintf(stderr, "ERROR: FRAMING");
}
void _BREAK_ERR() {
  fprintf(stderr, "INFO: BREAK_INT");
}

void _TCV_EMPTY() {
  TCV_READY = true;
}

void _TCV_WARN() {
  // fprintf(stderr, "INFO: TCV_EMPTY");
}

void (*const ERR_REPORT[])() = {
  &_RECV_READY_FN,
  &_OVERRUN_ERR,
  &_PARITY_ERR,
  &_FRAMING_ERR,
  &_BREAK_ERR,
  &_TCV_EMPTY,
  &_TCV_WARN,
};

// INTERNAL //

int _sp_read_status(uint8_t *st) {
  uint32_t msg;
  if (sys_inb(SP_UART_B + LSR, &msg))
    return 1;
  *st = (uint8_t) msg;
  return 0;
}

int _sp_read_int_id(uint8_t *id) {
  uint32_t msg = 0;
  if (sys_inb(SP_UART_B + IIR, &msg))
    return 1;
  *id = (uint8_t) msg;
  return 0;
}

int _sp_recv() {
  uint32_t msg = 0;
  if (sys_inb(SP_UART_B + RCV, &msg))
    return 1;
  RCV_READY = false;
  uint8_t bite = (uint8_t) msg;
  ring_buffer_push(IN_RING, &bite);
  return 0;
}

int _sp_trcv() {
  uint8_t value = 0;
  if (ring_buffer_pop(OUT_RING, &value)) {
    if (sys_outb(SP_UART_B + TCV, value))
      return 101; // Failed to send byte, even if allowed (panic worthy imo)
    TCV_READY = false;
  }
  return 0;
}

int _sp_status() {
  uint8_t st = 0;
  if (_sp_read_status(&st))
    return 1;
  for (uint8_t idx = 0; idx < 7; ++idx) {
    if (BIT(idx) & st) {
      ERR_REPORT[idx]();
    }
  }
  return 0;
}

int _sp_clear_ints() {
  return sys_outb(SP_UART_B + FCR, 7);
}

int _sp_config_read(uint8_t *lcr) {
  if (util_sys_inb(SP_UART_B + LCR, lcr))
    return 1;
  return 0;
}

int _sp_config_write(uint8_t lcr) {
  if (util_sys_outb(SP_UART_B + LCR, lcr))
    return 1;
  return 0;
}

uint16_t _sp_get_freq() {
  uint8_t lcr;
  if (_sp_config_read(&lcr))
    return 1;
  lcr |= BIT(7);
  if (_sp_config_write(lcr))
    return 1;
  uint8_t freq_lsb;
  if (util_sys_inb(SP_UART_B + DLLSB, &freq_lsb))
    return 1;
  uint8_t freq_msb;
  if (util_sys_inb(SP_UART_B + DLMSB, &freq_msb))
    return 1;
  lcr &= 0xFF - BIT(7);
  if (_sp_config_write(lcr))
    return 1;
  return (freq_msb << 8) | freq_lsb;
}

// INTERNAL END //

int sp_init(int32_t bit_no) {
  if (IN_RING != NULL || OUT_RING != NULL)
    return 101; // Already initialized
  IN_RING = ring_buffer_new(0, sizeof(uint8_t));
  OUT_RING = ring_buffer_new(0, sizeof(uint8_t));
  if (IN_RING == NULL || OUT_RING == NULL)
    return 1; // Failed to alloc (unreachable?)
  if (sys_irqsetpolicy(SP1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &bit_no))
    return 1;
  BIT_NO = bit_no;
  uint32_t ier;
  if (sys_inb(SP_UART_B + IER, &ier))
    return sp_terminate();
  ier &= 0xF0; // Turn off all Interrupts
  if (sys_outb(SP_UART_B + IER, ier | IER_RCVD))
    return sp_terminate();
  if (_sp_status())
    return sp_terminate();
  return 0;
}

int sp_terminate() {
  // Free Memory
  ring_buffer_free(IN_RING);
  ring_buffer_free(OUT_RING);
  // Avoid use after free
  IN_RING = NULL;
  OUT_RING = NULL;
  // Unsubscribe interrupt (Deprecated)
  if (sys_irqrmpolicy(&BIT_NO))
    return 2;
  return 1;
}

int sp_poll() {
  uint32_t err = 0;
  if ((err = _sp_status()))
    return err;
  if (RCV_READY)
    err |= _sp_recv();
  if (TCV_READY)
    err |= _sp_trcv();
  return err;
}

int sp_push_byte(uint8_t bite) {
  ring_buffer_push(OUT_RING, &bite);
  return 0;
}

bool sp_pop_byte(uint8_t *bite) {
  return ring_buffer_pop(IN_RING, bite);
}

int sp_busy_wait_for_packet(uint8_t *packet) {
  while (!sp_pop_byte(packet)) {
    tickdelay(7);
    if (sp_poll())
      return 1;
  }
  return 0;
}

int sp_busy_send_packet() {
  while (!ring_buffer_is_empty(OUT_RING)) {
    tickdelay(7);
    if (sp_poll())
      return 1;
  }
  return 0;
}

// WARN: DEPRECATED
int sp_ih() {
  uint8_t int_id = 0;
  if (_sp_read_int_id(&int_id))
    return 1; // failed to read
  if ((int_id & 1) == 0)
    return 0;                        // Did not generate int
  uint8_t cause = (int_id >> 1) & 7; // cause = bit 1,2,3
  switch (cause) {
    case 0: // Modem Status UNREACHABLE:
      return 0;
      break;
    case 1: // Transceiver Empty
      TCV_READY = true;
      return 0;
      break;
    case 4: // Character timeout
      return 1;
      break;
    case 2: // Received data
      return _sp_recv();
      break;
    case 3: // Line status
      return _sp_status();
      break;
    default:
      return 1;
  };
}

void sp_debug() {
  printf("IN_RING: \n");
  ring_buffer_dump_buffer(IN_RING);
  printf("TCV_READY: %d\n", TCV_READY);
  printf("RCV_READY: %d\n", RCV_READY);
  sp_print_config();
}

void sp_print_config() {
  uint8_t lcr;
  if (_sp_config_read(&lcr) == 0) {
    double freq = 115200.0 / _sp_get_freq();
    printf("Bit-rate: %d\n", (int) freq);
    uint8_t bits = lcr & 3;
    printf("We have %d bits per packet\n", 5 + bits);
    uint8_t stop = lcr & BIT(2) >> 2;
    printf("We have %d stop bits per packet\n", stop + 1);
    uint8_t parity = (lcr & (BIT(3) | BIT(4) | BIT(5))) >> 3;
    switch (parity) {
      case 0:
        printf("No parity\n");
        break;
      case 1:
        printf("Odd parity\n");
        break;
      case 3:
        printf("Even parity\n");
        break;
      case 5:
        printf("Always 1\n");
        break;
      case 7:
        printf("Always 0\n");
        break;
      default:
        printf("parity's fucked.\n");
    }
    uint8_t dlab = lcr >> 7;
    printf("DLAB activated = %d\n", dlab);
  }
}
