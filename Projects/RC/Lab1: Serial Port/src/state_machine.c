#include "../include/state_machine.h"
#include "../include/debug.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct conn *Connection;

typedef enum {
  /// Waiting for flag
  AwaitStart,
  /// Waiting for Address
  AwaitA,
  /// Waiting for Control
  AwaitC,
  /// Waiting for Bcc1
  AwaitBcc1,
  /// Waiting for flag in a supervision frame
  AwaiCtlFlag,
  /// User needs to provide a buffer to write the data to.
  ProvideBuf,
  /// Receiving Data
  Data,
  /// Escape the next byte
  Escaped,
  /// Parsing Successful
  ParseSucc,
  /// Connection Closed
  ConnClosed,
  /// Number of enum variants
  __ValidStateSize,
} ValidState;

typedef struct sm {
  u8 xor_accum;
  ValidState state;
  Frame frame;
} StateMachine;

struct conn {
  LinkLayerRole role;
};

StateMachine sm = {
    .xor_accum = 0,
    .state = ConnClosed,
    .frame = {0},
};

struct conn connection_singleton = {0};

Connection active_connection = NULL;

ValidState prev_state = ConnClosed;

ResultFrameState __await_start(u8 byte) {
  if (sm.state != AwaitStart) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }
  if (byte == FLAG) {
    sm.state = AwaitA;
  }
  return Ok;
}

ResultFrameState __await_a(u8 byte) {
  if (sm.state != AwaitA) {
    return ErrStateMachineBug;
  }

  if (active_connection == NULL) {
    return ErrClosed;
  }

  if (byte == FLAG) {
    return Ok;
  }

  if (byte != ARCV && byte != ATCV) {
    sm.state = AwaitStart;
    return Ok;
  }
  sm.frame.addr = byte;
  sm.xor_accum ^= byte;
  sm.state = AwaitC;
  return Ok;
}

ResultFrameState __await_c(u8 byte) {
  if (sm.state != AwaitC) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }
  FrameCommandField command = byte;
  sm.frame.tag = command;
  sm.xor_accum ^= byte;
  switch (command) {
  case Set:
  case UnNumberedAck:
  case Accept0:
  case Accept1:
  case Reject0:
  case Reject1:
  case Disconnect:
  case Info0:
  case Info1:
    sm.state = AwaitBcc1;
    return Ok;
  default:
    sm.frame.tag = 0;
    if (byte == FLAG) {
      sm.state = AwaitA;
      sm.xor_accum = 0;
      return Ok;
    } else {
      sm.state = AwaitStart;
      sm.xor_accum = 0;
      return Ok;
    }
  }
}

ResultFrameState __await_bcc1(u8 byte) {
  if (sm.state != AwaitBcc1) {
    return ErrStateMachineBug;
  }

  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }

  sm.xor_accum ^= byte;
  if (sm.xor_accum != 0) {
    sm.xor_accum = 0;
    if (byte == FLAG) {
      sm.state = AwaitA;
      sm.xor_accum = sm.frame.addr;
    } else {
      sm.state = AwaitStart;
      sm.xor_accum = 0;
    }
    return Ok;
  }
  switch (sm.frame.tag) {
  case Set:
  case UnNumberedAck:
  case Accept0:
  case Accept1:
  case Reject0:
  case Reject1:
  case Disconnect:
    sm.state = AwaiCtlFlag;
    return Ok;
  case Info0:
  case Info1:
    sm.state = ProvideBuf;
    return Ok;
  }
  return ErrStateMachineBug;
}

ResultFrameState __await_ctl_flag(u8 byte) {
  if (sm.state != AwaiCtlFlag) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }
  if (byte == FLAG) {
    sm.state = ParseSucc;
  } else {
    sm.state = AwaitStart;
  }
  return Ok;
}

ResultFrameState __data(u8 byte) {
  if (sm.state != Data) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }

  if (byte == FLAG) {
    if (sm.xor_accum == 0) {
      sm.state = ParseSucc;
      sm.frame.buf->written--;
      return Ok;
    } else {
      sm.state = AwaitA;
      sm.xor_accum = 0;
      sm.frame.buf->written = 0;
      return ErrReject;
    }
  }

  if (sm.frame.buf->size == sm.frame.buf->written) {
    sm.xor_accum = 0;
    sm.state = AwaitStart;
    sm.frame.buf->written = 0;
    return ErrReject;
  }

  if (byte == ESCAPE) {
    sm.state = Escaped;
    return Ok;
  }

  sm.xor_accum ^= byte;
  sm.frame.buf->buf[sm.frame.buf->written] = byte;
  sm.frame.buf->written++;
  return Ok;
}

ResultFrameState __escaped(u8 byte) {
  if (sm.state != Escaped) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }
  u8 res = byte ^ 0x20;
  sm.xor_accum ^= res;
  sm.frame.buf->buf[sm.frame.buf->written] = res;
  sm.frame.buf->written++;
  sm.state = Data;
  return Ok;
}

ResultFrameState __parse_succ(u8 byte) {
  if (sm.state != ParseSucc) {
    return ErrStateMachineBug;
  }
  if (active_connection == NULL) {
    sm.state = ConnClosed;
    return ErrClosed;
  }
  sm.frame.addr = 0;
  sm.frame.tag = 0;
  sm.frame.buf = NULL;
  sm.state = AwaitStart;
  __await_start(byte);
  return Ok;
}

ResultFrameState __conn_closed(u8 byte) { return ErrClosed; }

ResultFrameState __provide_buf(u8 byte) { return ErrProvideBuf; }

ResultFrameState (*const STATEFN[__ValidStateSize])(u8) = {
    [AwaitStart] = __await_start,
    [AwaitA] = __await_a,
    [AwaitC] = __await_c,
    [AwaitBcc1] = __await_bcc1,
    [AwaiCtlFlag] = __await_ctl_flag,
    [ProvideBuf] = __provide_buf,
    [Data] = __data,
    [Escaped] = __escaped,
    [ParseSucc] = __parse_succ,
    [ConnClosed] = __conn_closed,
};

ResultFrame sm_next(u8 byte) {
  debug_sp("0x%02X ", byte);
  ResultFrameState state = STATEFN[sm.state](byte);
  if (sm.state != prev_state) {
    debug_sm("%s -> %s (caused by 0x%02X)", state_to_string(prev_state),
             state_to_string(sm.state), byte);
    prev_state = sm.state;
  }
  switch (state) {
  case Ok:
    switch (sm.state) {
    case AwaitStart:
    case AwaitA:
    case AwaitC:
    case AwaitBcc1:
    case AwaiCtlFlag:
    case Data:
    case Escaped:
      return (ResultFrame){.state = Unfinished};
    case ProvideBuf:
      return (ResultFrame){.state = ErrProvideBuf};
    case ParseSucc:
      debug_sp("%s frame", frame_command_to_string(sm.frame.tag));
      return (ResultFrame){.state = Ok, .frame = sm.frame};
    case ConnClosed:
      return (ResultFrame){.state = ErrClosed};
    case __ValidStateSize:
      // Unreachable!
      exit(101);
    }
    break;
  case Unfinished:
    return (ResultFrame){.state = Unfinished};
  case ErrClosed:
    return (ResultFrame){.state = ErrClosed};
  case ErrReject:
    return (ResultFrame){.state = ErrReject, .frame = sm.frame};
  case ErrProvideBuf:
    return (ResultFrame){.state = ErrProvideBuf};
  case ErrStateMachineBug:
    exit(101);
  }
  return (ResultFrame){.state = ErrStateMachineBug};
}

void provide_buf(DecodedData *buf) {
  sm.frame.buf = buf;
  sm.state = Data;
}

void start_conn() {
  active_connection = &connection_singleton;
  sm.state = AwaitStart;
}

void close_conn() {
  active_connection = NULL;
  sm.state = ConnClosed;
}
