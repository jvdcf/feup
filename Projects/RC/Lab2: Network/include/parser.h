/**
 * @file parser.h
 * @brief FTP URL parser
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>
#include <arpa/inet.h>

#define MAX_INPUT_SIZE 60

typedef struct
{
  char user[MAX_INPUT_SIZE];
  char password[MAX_INPUT_SIZE];
  char host[MAX_INPUT_SIZE];
  struct sockaddr_in server_addr;
  char url_path[MAX_INPUT_SIZE];
} FTP_URL;

void parseFTPURL(char *url, FTP_URL *ftp);
void printFTP(FTP_URL *ftp);

#endif // _PARSER_H_
