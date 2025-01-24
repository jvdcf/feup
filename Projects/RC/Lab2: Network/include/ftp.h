/**
 * @file ftp.h
 * @brief (Incomplete) API for communication with FTP servers
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _FTP_H_
#define _FTP_H_

#include "parser.h"
#include <arpa/inet.h>

typedef struct ftpc FTPConn;

FTPConn *ftpConnect(FTP_URL *ftp);
void ftpRetrieve(FTPConn *ftp, char *path);
void ftpDisconnect(FTPConn *ftp);

#endif // _FTP_H_