// Link layer protocol implementation

#include "../include/link_layer.h"
#include "../include/alarm.h"
#include "../include/debug.h"
#include "../include/serial_port.h"
#include "../include/state_machine.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// MISC
#define _POSIX_SOURCE 1 // POSIX compliant source

// Internal Buffer
unsigned char ibuf[MAX_PAYLOAD_SIZE + 1] = {0};
LinkLayerRole role;
FrameCommandField prev_frame_type;

// Construct a supervision or unnumbered frame, given the address and control
// fields.
#define CREATE_BUF(addr, ctl)                                                  \
  (unsigned char[5]) { FLAG, (addr), (ctl), (addr) ^ (ctl), FLAG }

// Auxiliary wrapper to write bytes to the serial port safely.
int writeBytesSP(const unsigned char *bytes, int numBytes) {
  int written = 0;
  int total_written = 0;
  while (true) {
    // UNSAFE: ptr arithmetic. should be safe though.
    written =
        writeBytesSerialPort(bytes + total_written, numBytes - total_written);
    if (written == -1) {
      return -1;
    }
    total_written += written;
    if (total_written == numBytes) {
      return 0;
    }
  }
}

// Auxiliary function to build and write an information frame
bool write_are_we_info_1 = false;
int write_one_info_frame(const unsigned char *buf, const int bufSize) {
  unsigned char xor_accum = 0;
  FrameCommandField info = write_are_we_info_1 ? Info1 : Info0;
  unsigned char header[4] = {
      FLAG,
      ATCV,
      info,
      info ^ ATCV,
  };
  if (writeBytesSP(header, 4))
    return -1;
  int totalwritten = 0;
  int ibufwritten = 0;
  int ebufread = 0;
  // Using ibuf as the internal buffer for writting allows us to keep the size
  // smaller, since, by optimization, we should see many less FLAG or ESCAPE
  // than any other bytes, so it is safe to optimize like this. In case we can't
  // fit everything inside, we at most write twice.
  while (ebufread < bufSize) {
    xor_accum ^= buf[ebufread];
    if (buf[ebufread] != ESCAPE && buf[ebufread] != FLAG) {
      ibuf[ibufwritten] = buf[ebufread];
      ibufwritten++;
    } else {
      // this is always safe because the actual buffer size is MAX_PAYLOAD_SIZE
      // + 1, and we cap it at MAX_PAYLOAD_SIZE. Worst case we get a full
      // buffer.
      ibuf[ibufwritten] = ESCAPE;
      ibufwritten++;
      ibuf[ibufwritten] = buf[ebufread] ^ 0x20;
      ibufwritten++;
    }
    ebufread++;
    totalwritten++;
    // Flush
    if (ibufwritten >= MAX_PAYLOAD_SIZE) {
      if (writeBytesSP(ibuf, ibufwritten))
        return -1;
      ibufwritten = 0;
    }
  }
  // Flush remaining
  if (ibufwritten > 0) {
    if (writeBytesSP(ibuf, ibufwritten))
      return -1;
  }
  if (xor_accum != FLAG && xor_accum != ESCAPE) {
    if (writeBytesSP((unsigned char[]){xor_accum, FLAG}, 2))
      return -1;
  } else {
    if (writeBytesSP((unsigned char[]){ESCAPE, 0x20 ^ xor_accum, FLAG}, 2))
      return -1;
  }
  return 0;
}

// Auxiliary wrapper to read a byte with busy waiting.
static inline int readByteSP(unsigned char *byte) {
  int err = 0;
  while ((err = readByteSerialPort(byte)) == 0)
    ;
  return err;
}

////////////////////////////////////////////////
// LLOPEN
////////////////////////////////////////////////
int llopen(LinkLayer connectionParameters) {
  debug_ll("Opening connection...");
  if (openSerialPort(connectionParameters.serialPort,
                     connectionParameters.baudRate) < 0) {
    return -1;
  }
  alarmInit(connectionParameters.nRetransmissions,
            connectionParameters.timeout);
  role = connectionParameters.role;
  alarmReset();
  start_conn();

  if (role == LlTx) {
    while (!alarmMaxRetriesReached()) {
      if (!alarmIsEnabled()) {
        alarmOn();
        if (writeBytesSP(CREATE_BUF(ATCV, CMD_SET), 5))
          return -1;
        debug_ll("↑ SET; ⟳ UA");
      }
      unsigned char byte = 0;
      ResultFrame frame;
      int err = 0;
      if ((err = readByteSerialPort(&byte)) == 0)
        continue;
      if (err != 1)
        return -1;
      frame = sm_next(byte);
      switch (frame.state) {
      case Ok:
        prev_frame_type = frame.frame.tag;
        switch (frame.frame.tag) {
        case UnNumberedAck:
          alarmOff();
          debug_ll("↓ UA; connection established");
          return 1;
        default: // Set, Accept0, Accept1, Reject0, Reject1, Info0, Info1,
                 // Disconnect
          debug_ll("↓ %s; ignored", frame_command_to_string(frame.frame.tag));
          continue;
        }
      case ErrClosed:
        close_conn();
        return -1;
      case ErrStateMachineBug:
        exit(101);
      default: // Unfinished, ErrReject, ErrProvideBuf
        continue;
      }
    }
  }

  if (role == LlRx) {
    while (true) {
      unsigned char byte = 0;
      ResultFrame frame;
      int err = 0;
      if ((err = readByteSerialPort(&byte)) == 0)
        continue;
      if (err != 1)
        return -1;
      frame = sm_next(byte);
      switch (frame.state) {
      case Ok:
        prev_frame_type = frame.frame.tag;
        switch (frame.frame.tag) {
        case Set:
          debug_ll("↓ SET; ⇡ UA; connection established");
          if (writeBytesSP(CREATE_BUF(ATCV, CMD_UA), 5))
            return -1;
          return 1;
        default: // UnNumberedAck, Accept0, Accept1, Reject0, Reject1, Info0,
                 // Info1, Disconnect
          debug_ll("↓ %s; ignored", frame_command_to_string(frame.frame.tag));
          continue;
        }
      case ErrClosed:
        close_conn();
        return -1;
      case ErrStateMachineBug:
        exit(101);
      default: // Unfinished, ErrReject, ErrProvideBuf
        continue;
      }
    }
    writeBytesSerialPort(CREATE_BUF(ATCV, CMD_UA), 5);
  }

  close_conn();
  return -1;
}

////////////////////////////////////////////////
// LLWRITE
////////////////////////////////////////////////

int llwrite(const unsigned char *buf, int bufSize) {
  statisticsPacket();
  debug_ll("Writing %d bytes...", bufSize);
  while (!alarmMaxRetriesReached()) {
    if (!alarmIsEnabled()) {
      alarmOn();
      if (write_one_info_frame(buf, bufSize))
        return -1;
      debug_ll("↑ Info%d; ⟳ %s", write_are_we_info_1,
               write_are_we_info_1 ? "RR0" : "RR1");
    }
    unsigned char byte = 0;
    ResultFrame frame;
    int err = 0;
    if ((err = readByteSerialPort(&byte)) == 0)
      continue;
    if (err != 1)
      return -1;
    frame = sm_next(byte);
    switch (frame.state) {
    case Ok:
      prev_frame_type = frame.frame.tag;
      switch (frame.frame.tag) {
      case Accept0:
        if (!write_are_we_info_1) {
          debug_ll("↓ RR0; expected RR1; ignored");
          continue;
        }
        alarmReset();
        write_are_we_info_1 = false;
        debug_ll("↓ RR0; frame transmission successful");
        return bufSize;
      case Accept1:
        if (write_are_we_info_1) {
          debug_ll("↓ RR1; expected RR0; ignored");
          continue;
        }
        alarmReset();
        write_are_we_info_1 = true;
        debug_ll("↓ RR1; frame transmission successful");
        return bufSize;
      case Reject0:
      case Reject1:
        debug_ll("↓ %s; ⇡ Info%d", frame_command_to_string(frame.frame.tag),
                 write_are_we_info_1);
        statisticsRetransmission();
        alarmReset();
        continue;
      default: // Set, Info0, Info1, Disconnect
        debug_ll("↓ %s; ignored", frame_command_to_string(frame.frame.tag));
        continue;
      }
    case ErrClosed:
      return -1;
    case ErrStateMachineBug:
      exit(101);
    case ErrProvideBuf:
      exit(169);
    default: // Unfinished, ErrReject, ErrProvideBuf
      continue;
    }
  }

  return -1;
}

////////////////////////////////////////////////
// LLREAD
////////////////////////////////////////////////
bool read_are_we_info_1 = false;

int llread(unsigned char *packet) {
  statisticsPacket();
  debug_ll("⟳ Info%d; reading a new frame...", read_are_we_info_1);
  DecodedData data = {.buf = ibuf, .size = MAX_PAYLOAD_SIZE + 1, .written = 0};
  ResultFrame frame;
  unsigned char byte = 0;
  while (true) {
    int err = readByteSP(&byte);
    if (err != 1)
      return err;
    // advance the state machine
    frame = sm_next(byte);
    switch (frame.state) {
    case Ok:
      prev_frame_type = frame.frame.tag;
      // See what type of frame it is and handle it.
      switch (frame.frame.tag) {
      case Set:
        debug_ll("↓ SET; ⇡ UA; connection established");
        if (writeBytesSP(CREATE_BUF(ATCV, UnNumberedAck), 5))
          return -1;
        continue;
      case UnNumberedAck:
      case Accept0:
      case Accept1:
      case Reject0:
      case Reject1:
      case Disconnect:
        debug_ll("↓ %s; ignoring...", frame_command_to_string(frame.frame.tag));
        continue;
      case Info0:
        if (read_are_we_info_1) {
          debug_ll("↓ Info0; expected Info1; ignored");
          continue;
        }
        debug_ll("↓ Info0; ⇡ RR1");
        memcpy(packet, data.buf, data.written);
        read_are_we_info_1 = true;
        if (writeBytesSP(CREATE_BUF(ATCV, Accept1), 5))
          return -1;
        return data.written;
      case Info1:
        if (!read_are_we_info_1) {
          debug_ll("↓ Info1; expected Info0; ignored");
          continue;
        }
        debug_ll("↓ Info1; ⇡ RR0");
        memcpy(packet, data.buf, data.written);
        read_are_we_info_1 = false;
        if (writeBytesSP(CREATE_BUF(ATCV, Accept0), 5))
          return -1;
        return data.written;
      }
    case Unfinished:
      continue;
    case ErrClosed:
      return -1;
    case ErrReject:
      statisticsRetransmission();
      if (frame.frame.tag == Info0) {
        debug_ll("⇡ REJ0; ⟳ Info0; error in transmission");
        if (writeBytesSP(CREATE_BUF(ATCV, Reject0), 5))
          return -1;
      } else {
        debug_ll("⇡ REJ1; ⟳ Info1; error in transmission");
        if (writeBytesSP(CREATE_BUF(ATCV, Reject1), 5))
          return -1;
      }
      continue;
    case ErrProvideBuf:
      provide_buf(&data);
      continue;
    case ErrStateMachineBug:
      exit(101);
    }
  }
}

////////////////////////////////////////////////
// LLCLOSE
////////////////////////////////////////////////
int llclose(int showStatistics) {
  debug_ll("Closing connection...");
  if (role == LlTx) {
    alarmReset();
    while (!alarmMaxRetriesReached()) {
      if (!alarmIsEnabled()) {
        alarmOn();
        if (writeBytesSP(CREATE_BUF(ATCV, CMD_DISC), 5))
          return -1;
        debug_ll("↑ DISC; ⟳ DISC");
      }
      unsigned char byte = 0;
      ResultFrame frame;
      int err = 0;
      if ((err = readByteSerialPort(&byte)) == 0)
        continue;
      if (err != 1)
        return -1;
      frame = sm_next(byte);
      switch (frame.state) {
      case Ok:
        prev_frame_type = frame.frame.tag;
        switch (frame.frame.tag) {
        case Disconnect:
          alarmOff();
          debug_ll("↓ DISC; ⇡ UA; connection closed");
          if (writeBytesSP(CREATE_BUF(ARCV, CMD_UA), 5))
            return -1;
          close_conn();
          if (closeSerialPort() == -1)
            return -1;
          if (showStatistics)
            statisticsPrint(true);
          return 1;
          continue;
        default: // Set, Accept0, Accept1, Reject0, Reject1, Info0, Info1,
                 // Disconnect
          debug_ll("↓ %s; ignored", frame_command_to_string(frame.frame.tag));
          continue;
        }
      case ErrClosed:
        return -1;
      case ErrStateMachineBug:
        exit(101);
      default: // Unfinished, ErrReject, ErrProvideBuf
        continue;
      }
    }
    return -1;
  }

  if (role == LlRx) {
    bool discReceived = false;
    while (true) {
      unsigned char byte = 0;
      ResultFrame frame;
      int err = 0;
      if ((err = readByteSerialPort(&byte)) == 0)
        continue;
      if (err != 1)
        return -1;
      frame = sm_next(byte);
      switch (frame.state) {
      case Ok:
        prev_frame_type = frame.frame.tag;
        switch (frame.frame.tag) {
        case Disconnect:
          debug_ll("↓ DISC; ⇡ DISC; ⟳ UA");
          discReceived = true;
          if (writeBytesSP(CREATE_BUF(ARCV, CMD_DISC), 5))
            return -1;
          continue;
        case UnNumberedAck:
          if (discReceived) {
            debug_ll("↓ UA; connection closed");
            close_conn();
            if (closeSerialPort() == -1)
              return -1;
            if (showStatistics)
              statisticsPrint(false);
            return 1;
          }
          debug_ll("↓ UA; expected DISC; ignore");
          continue;
        default: // Set, Accept0, Accept1, Reject0, Reject1, Info0, Info1
          debug_ll("↓ %s; ignored", frame_command_to_string(frame.frame.tag));
          continue;
        }
      case ErrClosed:
        return -1;
      case ErrStateMachineBug:
        exit(101);
      default: // Unfinished, ErrReject, ErrProvideBuf
        continue;
      }
    }
    return -1;
  }
  return -1;
}
