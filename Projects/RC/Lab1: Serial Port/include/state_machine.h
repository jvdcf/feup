/**
 * @file state_machine.h
 * @brief General State Machine abstraction
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_
#include "field.h"
#include "link_layer.h"
#include <stdbool.h>

typedef unsigned char u8;
typedef struct conn* Connection;

typedef struct {
  u8 *buf;
  unsigned short size;
  unsigned short written;  
} DecodedData;


typedef enum {
  /// Transceiver initiated
  TcvInit = 1,
  /// Receiver initiated
  RcvInit = 2,
} AddressTransaction;


typedef struct {
   AddressTransaction addr;
   FrameCommandField tag;
   // Optional, auto NULL
   DecodedData* buf; 
} Frame;

typedef enum {
    Ok,
    Unfinished,
    ErrClosed,
    ErrReject,
    ErrProvideBuf,
    ErrStateMachineBug,
} ResultFrameState;



typedef struct {
    ResultFrameState state;
    // Optional, auto NULL
    union {
      // OK
      struct { Frame frame; };
    };
} ResultFrame;




/**
 * 
**/
ResultFrame sm_next(u8 byte);

void provide_buf(DecodedData* buf);

void start_conn();

void close_conn();
 
#endif // _STATE_MACHINE_H_
