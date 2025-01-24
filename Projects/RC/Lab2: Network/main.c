/**
 * @file main.c
 * @brief Entry point of the FTP client program
 * @authors Guilherme Matos, João Ferreira
 */

#include "include/parser.h"
#include "include/ftp.h"
#include "include/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_URL_SIZE MAX_INPUT_SIZE * 5

int main(int argc, char *argv[])
{
  char ftpURL[MAX_URL_SIZE];
  switch (argc)
  {
  case 1:
    info("No arguments provided; please enter the FTP URL:");
    int size = read(STDIN_FILENO, ftpURL, MAX_URL_SIZE);
    ftpURL[size - 1] = '\0';
    break;

  case 2:
    strcpy(ftpURL, argv[1]);
    break;

  default:
    error("Invalid arguments\n  Usage: download ftp://[<user>:<password>@]<host>/<url-path>");
  }

  FTP_URL ftp;
  parseFTPURL(ftpURL, &ftp);
  printFTP(&ftp);

  FTPConn *auth = ftpConnect(&ftp);
  ftpRetrieve(auth, ftp.url_path);
  ftpDisconnect(auth);

  return 0;
}
