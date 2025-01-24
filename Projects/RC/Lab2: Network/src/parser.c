/**
 * @file parsec.c
 * @brief FTP URL parser implementation
 * @authors Guilherme Matos, João Ferreira
 */

#include "../include/parser.h"
#include "../include/debug.h"
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 21

void getIPFromHost(char *host, struct sockaddr_in *addr)
{
  struct hostent *h;

  if ((h = gethostbyname(host)) == NULL)
    error("Failed to get IP from host");

  char *ip = inet_ntoa(*((struct in_addr *)h->h_addr_list[0]));
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(ip);
  addr->sin_port = htons(SERVER_PORT);
}

/**
 * @warning The use of sscanf without field width specifiers is unsafe (without AddressSanitizer)!
 * If the field is larger than MAX_INPUT_SIZE, an buffer overflow will occur.
 * AddressSanitizer will detect this error.
 */
void parseFTPURL(char *url, FTP_URL *ftp)
{
  // With user and password: ftp://[<user>:<password>@]<host>/<url-path>
  if (sscanf(url, "ftp://%[^:]:%[^@]@%[^/]/%s", ftp->user, ftp->password, ftp->host, ftp->url_path) == 4)
  {
    getIPFromHost(ftp->host, &ftp->server_addr);
    return;
  }

  // With only host and path: ftp://<host>/<url-path>
  if (sscanf(url, "ftp://%[^/]/%s", ftp->host, ftp->url_path) == 2)
  {
    strcpy(ftp->user, "anonymous");
    strcpy(ftp->password, "anonymous");
    getIPFromHost(ftp->host, &ftp->server_addr);
    return;
  }

  error("Invalid FTP URL");
}

void printFTP(FTP_URL *ftp)
{
  printf("FTP URL:\n");
  printf("User: %s\n", ftp->user);
  printf("Password: %s\n", ftp->password);
  printf("Host: %s\n", ftp->host);
  printf("IP: %s:%d\n", inet_ntoa(ftp->server_addr.sin_addr), ntohs(ftp->server_addr.sin_port));
  printf("URL path: %s\n", ftp->url_path);
}
