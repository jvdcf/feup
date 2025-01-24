// Application layer protocol implementation

#include "../include/application_layer.h"
#include "../include/debug.h"
#include "../include/field.h"
#include "../include/link_layer.h"
#include <stdbool.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// Maximum packet data field size per payload
#define MAX_DATA_SIZE MAX_PAYLOAD_SIZE - 4

// Initialize link layer configuration via the input parameters
LinkLayer linkLayerInit(const char *serialPort, const char *role, int baudRate,
                        int nTries, int timeout) {
  LinkLayer serialConf;
  strcpy(serialConf.serialPort, serialPort);
  serialConf.baudRate = baudRate;
  serialConf.nRetransmissions = nTries;
  serialConf.timeout = timeout;
  if (strcmp(role, "tx") == 0)
    serialConf.role = LlTx;
  else if (strcmp(role, "rx") == 0)
    serialConf.role = LlRx;
  else
    error("Wrong role parameter");
  return serialConf;
}

// Auxiliary function to get the file size via stat()
unsigned long long getFileSize(const char *filename) {
  struct stat fileStat;
  if (stat(filename, &fileStat) < 0)
    error("Failed to get file size");
  return fileStat.st_size;
}

unsigned char min_num_bytes(unsigned long long value) {
  unsigned char cnt = 1;
  value >>= 8;
  while (value) {
    value >>= 8;
    ++cnt;
  }
  return cnt;
}

// Send a control packet with the file size. It can be a start or a end packet.
void sendControlPacket(unsigned long long fileSize, const char *filename,
                       bool isStart) {
  if (isStart)
    debug_al("Sending Control Start packet...");
  else
    debug_al("Sending Control End packet...");
  unsigned char min_bytes = min_num_bytes(fileSize);
  unsigned char filename_len = strlen(filename) > 255 ? 255 : strlen(filename);
  unsigned char controlPacket[3 + 8 + (2 + 255)] = {0};
  if (isStart)
    controlPacket[0] = CTRL_START;
  else
    controlPacket[0] = CTRL_END;
  controlPacket[1] = CTRL_FILE_SIZE;
  controlPacket[2] = min_bytes;
  for (int i = 0; i < min_bytes; ++i) {
    controlPacket[3 + i] = fileSize >> (8 * (min_bytes - i - 1));
  };
  controlPacket[3 + min_bytes] = CTRL_FILE_NAME;
  controlPacket[3 + min_bytes + 1] = filename_len;
  memcpy(&controlPacket[3 + min_bytes + 2], filename, filename_len);

  int bytesSent = llwrite(controlPacket, 3 + min_bytes + 2 + filename_len);
  if (bytesSent == -1)
    error("Failed to write Control packet to the serial port");
  if (bytesSent != 3 + min_bytes + 2 + filename_len)
    error("The number of bytes sent is not the same number of bytes of the "
          "control packet");
}

// Send a data packet to the link layer with the data
void sendDataPacket(unsigned char *data, int dataSize, int sequenceNumber) {
  debug_al("Sending Data packet nº%02d...", sequenceNumber);
  unsigned char dataPacket[MAX_PAYLOAD_SIZE] = {0};
  dataPacket[0] = CTRL_DATA;
  dataPacket[1] = sequenceNumber;
  dataPacket[2] = dataSize >> 8;
  dataPacket[3] = dataSize;
  memcpy(dataPacket + 4, data, dataSize);
  int bytesSent = llwrite(dataPacket, dataSize + 4);
  if (bytesSent == -1)
    error("Failed to write Data packet to the serial port");
  if (bytesSent != dataSize + 4)
    error("The number of bytes sent is not the same number of bytes of the "
          "data packet");
}

// Read a Control Start packet, check for errors and return the file size
unsigned long long readControlStartPacket(unsigned char *buf) {
  debug_al("Waiting for Control Start packet...");
  int bytesRead = llread(buf);
  if (bytesRead == -1)
    error("Failed to read Control Start packet");
  if (buf[0] != CTRL_START)
    error("Received packet is not a Control Start packet");
  if (buf[1] != CTRL_FILE_SIZE)
    error("Control Start type field is not a file size type");
  unsigned char paramLength = buf[2];
  if (bytesRead < 3 + paramLength)
    error("Control Start packet has less bytes than expected");
  unsigned long long fileSize = 0;
  for (int i = 0; i < paramLength; i++) {
    fileSize = (fileSize << 8) | buf[3 + i];
  }
  return fileSize;
}

void checkControlEndPacket(unsigned char *buf, unsigned long long fileSize,
                           const char *filename) {
  if (buf[0] != CTRL_END)
    error("Received packet is not a Control End packet");
  if (buf[1] != CTRL_FILE_SIZE)
    error("Control End type field is not a file size type");
  unsigned char paramLength = buf[2];
  unsigned long long int endFileSize = 0;
  for (int i = 0; i < paramLength; i++) {
    endFileSize = (endFileSize << 8) | buf[3 + i];
  }
  if (endFileSize != fileSize)
    error("Received file size does not match the file size in the Control "
          "Start packet");
  if (fileSize != getFileSize(filename))
    error("File size does not match the received file size");
}

// =============================================================================

void applicationLayer(const char *serialPort, const char *role, int baudRate,
                      int nTries, int timeout, const char *filename) {
  LinkLayer serialConf =
      linkLayerInit(serialPort, role, baudRate, nTries, timeout);
  if (llopen(serialConf) != 1)
    error("Failed to open serial port");
  unsigned long long fileSize;

  if (serialConf.role == LlTx) { // Transmitter: Read file and write
    fileSize = getFileSize(filename);
    sendControlPacket(fileSize, filename, true);

    FILE *readFile = fopen(filename, "r");
    if (readFile == NULL)
      error("Failed to open file");

    // Read from file and send data packets, 10KB at a time
    unsigned char fileData[MAX_DATA_SIZE] = {0};
    int remainingBytes = fileSize;
    for (int sequenceNumber = 0; remainingBytes > 0; sequenceNumber = (sequenceNumber + 1) % 100) {
      int bytesRead = fread(fileData, 1, MAX_DATA_SIZE, readFile);
      debug_al("Read %d bytes from file...", bytesRead);
      sendDataPacket(fileData, bytesRead, sequenceNumber);
      remainingBytes -= bytesRead;
    }

    fclose(readFile);
    sendControlPacket(fileSize, filename, false);
  }

  if (serialConf.role == LlRx) { // Receiver: Read and write to file
    unsigned char buf[MAX_PAYLOAD_SIZE] = {0};
    fileSize = 0;
    FILE *writeFile = fopen(filename, "w");
    if (writeFile == NULL)
      error("Failed to open file");
    unsigned char controlByte = 0;

    fileSize = readControlStartPacket(buf);

    // Read data packets and write to file
    unsigned char sequenceNumber = 0;
    while (true) {
      debug_al("Waiting for Data packet nº%02d...", sequenceNumber);
      int bytesRead = llread(buf);
      if (bytesRead == -1)
        error("Failed to read Data/End packet");
      if (bytesRead < 7)
        error("Data/End packet has less bytes than expected");
      controlByte = buf[0];
      if (controlByte == CTRL_END) {
        debug_al("Received Control End packet; verifying values...");
        break;
      }
      if (controlByte != CTRL_DATA)
        error("Received packet is not a data packet nor a control end packet");
      if (buf[1] != sequenceNumber)
        error("Received packet has wrong sequence number");
      unsigned short dataSize = (buf[2] << 8) | buf[3];
      debug_al("Writing %d bytes to file...", dataSize);
      fwrite(buf + 4, sizeof(char), dataSize, writeFile);
      sequenceNumber = (sequenceNumber + 1) % 100;
    }

    fclose(writeFile);
    checkControlEndPacket(buf, fileSize, filename);
  }

  if (llclose(TRUE) != 1)
    error("Failed to close serial port");
  if (serialConf.role == LlTx)
    debug_al("File sent successfully!");
  else
    debug_al("File received successfully!");
}
