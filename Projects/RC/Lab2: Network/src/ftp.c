/**
 * @file ftp.c
 * @brief (Incomplete) API implementation for FTP communication
 * @authors Guilherme Matos, João Ferreira
 */

#include "../include/ftp.h"
#include "../include/debug.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_MSG_LENGTH 30
#define MAX_BUF_LENGHT 1024 * 1024

typedef struct serv_msg {
  unsigned short code;
  union {
    bool head_ending;
    struct sockaddr_in addr;
    int nbytes;
  };
} ServMsg;

// ============================================================================

void writeToServer(int socketfd, char *msg) {
  int bytes = write(socketfd, msg, strlen(msg));
  if (bytes < 0)
    error("Failed to write to the socket");
}

int readMessageFromServer(int socketfd, ServMsg *msg) {
  char header[4] = {0};
  int head = 0;
  int err = 0;
  while (head < 3) {
    head += (err = read(socketfd, header + head, (sizeof(header) - 1) - head));
    if (err < 0)
      error("Error: could not read from FD: %d", socketfd);
  }
  printf("%s", header);
  msg->code = (((unsigned char)header[0] - '0') * 100) + 
              (((unsigned char)header[1] - '0') * 10) +
              (((unsigned char)header[2] - '0') * 1);
  if (msg->code == 227) {
    char last = 0;
    char curr = 0;
    unsigned char addr[6] = {0};
    char idx = 0;
    bool is_parsing_addr = false;
    do {
      if (idx >= 6) {
        info("Tried to read too many bytes.");
        idx = 0;
      }
      last = curr;
      int err = read(socketfd, &curr, 1);
      if (err < 0)
        error("TODO: I'll think of this message later");
      if (err == 0)
        continue;
      printf("%c", curr);
      if (curr == '(') {
        is_parsing_addr = true;
      } else if (curr == ')') {
        is_parsing_addr = false;
        idx = 0;
      } else if (curr == ',' && last != ',') {
        idx++;
      } else {
        if (!isdigit(curr)) {
          continue;
        }
        addr[idx] *= 10;
        addr[idx] += curr - '0';
      }
    } while (curr != '\n' && last != '\r');
    if (is_parsing_addr)
      error("Incomplete address parsing");
    unsigned short port = (addr[4] << 8) + addr[5];
    char ip[21] = {0};
    sprintf(ip, "%hhu.%hhu.%hhu.%hhu", addr[0], addr[1], addr[2], addr[3]);
    msg->addr.sin_family = AF_INET;
    msg->addr.sin_addr.s_addr = inet_addr(ip);
    msg->addr.sin_port = htons(port);
  } else if (msg->code == 150) {
    char last = 0;
    char curr = 0;
    bool is_parsing = false;
    msg->nbytes = 0;
    do {
      last = curr;
      int err = read(socketfd, &curr, 1);
      if (err < 0)
        error("TODO: I'll think of this message later");
      if (err == 0)
        continue;

      printf("%c", curr);
      if (curr == '(') {
        is_parsing = true;
      } else if (curr == ' ') {
        is_parsing = false;
      } else {
        if (!isdigit(curr)) {
          continue;
        }
        msg->nbytes *= 10;
        msg->nbytes += curr - '0';
      }
    } while (curr != '\n' && last != '\r');
  } else {
    char last = 0;
    char curr = 0;
    msg->head_ending = true;
    do {
      last = curr;
      int err = read(socketfd, &curr, 1);
      if (err < 0)
        error("TODO: I'll think of this message later");
      if (err == 0)
        continue;
      if (curr != '\r' && curr != '\n') {
        msg->head_ending = false;
      }
      printf("%c", curr);
    } while (curr != '\n' && last != '\r');
  }
  if (!isdigit(header[0]) || !isdigit(header[1]) || !isdigit(header[2])) {
    msg->code = 230;
  }
  return 0;
}

// ============================================================================

typedef struct ftpc {
  int socketfd;
  struct sockaddr_in server_addr;
} FTPConn;

FTPConn *ftpConnect(FTP_URL *ftp) {
  FTPConn *auth = (FTPConn *)malloc(sizeof(FTPConn));
  ServMsg msg = {0};
  // Open TCP socket
  auth->socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (auth->socketfd < 0)
    error("Failed to open a socket");

  // Connect to the server
  int err = connect(auth->socketfd, (struct sockaddr *)&ftp->server_addr,
                    sizeof(ftp->server_addr));
  if (err < 0)
    error("Failed to connect to the server");

  // Authenticate
  char user[MAX_INPUT_SIZE + 5 + 2 + 1] = {0};
  sprintf(user, "USER %s\r\n", ftp->user);
  writeToServer(auth->socketfd, user);
  info("%s", user);
  // Read Auth user response

  do {
    readMessageFromServer(auth->socketfd, &msg);
  } while (msg.code == 220);
  if (msg.code != 331) {
    error("Expected a password prompt (331), got (%d)", msg.code);
  }

  char password[MAX_INPUT_SIZE + 5 + 2 + 1] = {0};
  sprintf(password, "PASS %s\r\n", ftp->password);
  writeToServer(auth->socketfd, password);
  info("%s", password);
  // Read Auth password response
  readMessageFromServer(auth->socketfd, &msg);
  if (msg.code != 230) {
    if (msg.code == 530) {
      error("Authentication Rejected. Expected (230), got (%d)", msg.code);
    }
    error("Authentication failed. Expected (230), got (%d)", msg.code);
  }
  info("Authentication successful!");
  return auth;
}

void read_from_passive(int passvdf, unsigned int bufsize, unsigned int bytes_to_read,
                       char *filename) {
  char *buf = calloc(bufsize, sizeof(char));
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    error("Unable to open file: '%s'", filename);
  }
  int nread;
  while ((nread = read(passvdf, buf, bufsize)) && bytes_to_read > 0) {
    if (nread < 0) {
      error("Encountered error while reading file.");
    }
    bytes_to_read -= nread;
    fwrite(buf, 1, nread, file);
  }
  fclose(file);
  free(buf);
  buf = NULL;
  file = NULL;
}

void ftpRetrieve(FTPConn *ftp, char *path) {
  ServMsg msg = {0};
  // DONE: send PASV
  char passive[] = "PASV\r\n";
  writeToServer(ftp->socketfd, passive);
  info("%s", passive);
  do {
    
    readMessageFromServer(ftp->socketfd, &msg);
  } while (msg.code == 230);
  if (msg.code != 227)
    error("Expected to enter passive mode, but received (%d)", msg.code);

  info("Trying to establish connection...");
  // DONE: create socket
  int passvfd = socket(AF_INET, SOCK_STREAM, 0);
  // DONE: connect to socket
  int err = connect(passvfd, (struct sockaddr *)&msg.addr, sizeof(msg.addr));
  if (err < 0)
    error("Failed to connect to provided address.");

  info("Established passive connection.");
  // DONE: RETR ftp.path
  char file[MAX_INPUT_SIZE + 5 + 2 + 1] = {0};
  sprintf(file, "RETR %s\r\n", path);
  writeToServer(ftp->socketfd, file);
  info("%s", file);

  readMessageFromServer(ftp->socketfd, &msg);
  if (msg.code < 100 || msg.code >= 200)
    error("Preliminary message not in range");
  info("Received preliminary message.");
  int bytes_to_read = msg.nbytes;
  int bufsize = bytes_to_read < MAX_BUF_LENGHT ? bytes_to_read : MAX_BUF_LENGHT;
  char *filename = strrchr(path, '/');
  if (filename == NULL) {
    filename = path;
  } else {
    filename++;
  }
  // DONE: Continuously append to file
  info("Started passive download.");
  read_from_passive(passvfd, bufsize, bytes_to_read, filename);
  info("Finished passive download.");
  // DONE: Receive messages from the server
  do {
    readMessageFromServer(ftp->socketfd, &msg);
  } while (msg.code < 200);
}

void ftpDisconnect(FTPConn *ftp) {
  // Send QUIT command
  writeToServer(ftp->socketfd, "QUIT\r\n");
  ServMsg msg = {0};
  // TODO: Read 221 Goodbye

  readMessageFromServer(ftp->socketfd, &msg);
  if (msg.code != 221) {
    error("Expected a goodbye message (221), got (%d) (goodvolta?)", msg.code);
  }
  // Close the socket
  if (close(ftp->socketfd) < 0)
    error("Failed to close the socket");

  free(ftp);
  ftp = NULL;
}
