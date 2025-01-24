/**
 * @file field.h
 * @brief Field definitions used inside frames 
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _FIELD_H_
#define _FIELD_H_

// Flag: Synchronisation for the start or end of a frame
#define FLAG        0x7E 
// Address field in frames that are commands sent by the Transmitter or replies sent by the Receiver
#define ATCV        0x03 
// Address field in frames that are commands sent by the Receiver or replies sent by the Transmitter
#define ARCV        0x01 
// SET frame: sent by the transmitter to initiate a connection
#define CMD_SET     0x03 
// UA frame: confirmation to the reception of a valid supervision frame
#define CMD_UA      0x07 
// RR0 frame: indication sent by the Receiver that it is ready to receive an information frame number 0
#define CMD_RR0     0xAA 
// RR1 frame: indication sent by the Receiver that it is ready to receive an information frame number 1
#define CMD_RR1     0xAB 
// REJ0 frame: indication sent by the Receiver that it rejects an information frame number 0 (detected an error)
#define CMD_REJ0    0x54 
// REJ1 frame: indication sent by the Receiver that it rejects an information frame number 1 (detected an error)
#define CMD_REJ1    0x55 
// DISC frame to indicate the termination of a connection
#define CMD_DISC    0x0B 
// Information frame number 0
#define CMD_INF0    0x00 
// Information frame number 1
#define CMD_INF1    0x80 
// Escape character (see slide 17)
#define ESCAPE      0x7D


#define __FCFsize 9 
// The types of frames that can be sent either by the Receiver or the Transmitter
typedef enum {
  Set = CMD_SET,
  UnNumberedAck = CMD_UA,
  Accept0 = CMD_RR0,
  Accept1 = CMD_RR1,
  Reject0 = CMD_REJ0,
  Reject1 = CMD_REJ1,
  Disconnect = CMD_DISC,
  Info0 = CMD_INF0,
  Info1 = CMD_INF1,
} FrameCommandField; 


// Control field for the start of a transmission
#define CTRL_START  0x01
// Control field for data packets
#define CTRL_DATA   0x02
// Control field for the end of a transmission
#define CTRL_END    0x03
// Type field for the file size in a control packet
#define CTRL_FILE_SIZE 0x00
// File name
#define CTRL_FILE_NAME 0x01

#endif // _FIELD_H_
