#include "../include/debug.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void debug_al(const char *msg, ...) {
#ifdef DEBUG_APPLICATION_LAYER
  va_list args;
  va_start(args, msg);
  printf("[AL] ");
  vprintf(msg, args);
  printf("\n");
  va_end(args);
#endif
}

void debug_ll(const char *msg, ...) {
#ifdef DEBUG_LINK_LAYER
  va_list args;
  va_start(args, msg);
  printf("[LL] ");
  vprintf(msg, args);
  printf("\n");
  va_end(args);
#endif
}

void debug_am(const char *msg, ...) {
#ifdef DEBUG_ALARM
  va_list args;
  va_start(args, msg);
  printf("[ALARM] ");
  vprintf(msg, args);
  printf("\n");
  va_end(args);
#endif
}

void debug_sm(const char *msg, ...) {
#ifdef DEBUG_STATE_MACHINE
  va_list args;
  va_start(args, msg);
  printf("[SM] ");
  vprintf(msg, args);
  printf("\n");
  va_end(args);
#endif
}

void debug_sp(const char *msg, ...) {
#ifdef DEBUG_SERIAL_PORT
  va_list args;
  va_start(args, msg);
  // printf("[SP] ");
  vprintf(msg, args);
  // printf("\n");
  va_end(args);
#endif
}

void error(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  fprintf(stderr, "[ERROR] ");
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(-1);
}

const char *frame_command_to_string(FrameCommandField command) {
  switch (command) {
  case Set:
    return "Set";
  case UnNumberedAck:
    return "UA";
  case Accept0:
    return "RR0";
  case Accept1:
    return "RR1";
  case Reject0:
    return "REJ0";
  case Reject1:
    return "REJ1";
  case Disconnect:
    return "DISC";
  case Info0:
    return "Info0";
  case Info1:
    return "Info1";
  }
  return "Unknown";
}

const char *state_to_string(int state) {
  switch (state) {
  case 0:
    return "AwaitStart";
  case 1:
    return "AwaitA";
  case 2:
    return "AwaitC";
  case 3:
    return "AwaitBcc1";
  case 4:
    return "AwaiCtlFlag";
  case 5:
    return "ProvideBuf";
  case 6:
    return "Data";
  case 7:
    return "Escaped";
  case 8:
    return "ParseSucc";
  case 9:
    return "ConnClosed";
  }
  return "Unknown";
}

// =============================================================================

Statistics statistics = {0, 0, 0};

void statisticsPacket() { statistics.packets++; }

void statisticsRetransmission() { statistics.retransmissions++; }

void statisticsTimeout() { statistics.timeouts++; }

void statisticsPrint(bool isTransmitter) {
  printf("Statistics:\n");
  if (isTransmitter) {
    printf("- %d packets sent\n", statistics.packets);
    printf("- %d retransmissions made\n", statistics.retransmissions);
    printf("- %d timeouts\n", statistics.timeouts);
  } else {
    printf("- %d packets received\n", statistics.packets);
    printf("- %d retransmissions requested\n", statistics.retransmissions);
  }
}
