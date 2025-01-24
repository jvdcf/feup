/**
 * @file debug.h
 * @brief Auxiliary functions for printing debug messages
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

// Comment/uncomment these lines to disable/enable debug messages for each layer or component
#define DEBUG_APPLICATION_LAYER
// #define DEBUG_LINK_LAYER
#define DEBUG_ALARM
// #define DEBUG_STATE_MACHINE
// #define DEBUG_SERIAL_PORT

#include "field.h"
#include "state_machine.h"

// Print an optional debug message for the application layer
void debug_al(const char *msg, ...);
// Print an optional debug message for the link layer
void debug_ll(const char *msg, ...);
// Print an optional debug message for the alarm
void debug_am(const char *msg, ...);
// Print an optional debug message for the state machine
void debug_sm(const char *msg, ...);
// Print an optional debug message for the serial port
void debug_sp(const char *msg, ...);
// Print a mandatory error message and exit the program
void error(const char *msg, ...);
// Convert a frame type into a human-readable string
const char *frame_command_to_string(FrameCommandField command);
// Convert a state into a human-readable string
const char *state_to_string(int state);

// =============================================================================

// Struct for the statistics printed in llclose()
typedef struct {
  unsigned int packets;           // Number of packets sent/received
  unsigned int retransmissions;   // Number of retransmissions made
  unsigned int timeouts;          // Number of timeouts made by the alarm
} Statistics;

// Increment the number of packets sent/received
void statisticsPacket();
void statisticsRetransmission();
void statisticsTimeout();
void statisticsPrint(bool isTransmitter);

#endif // _DEBUG_H_