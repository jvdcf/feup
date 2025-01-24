/**
 * @file debug.c
 * @brief Implementation of debug messages
 * @authors Guilherme Matos, João Ferreira
 */

#include "../include/debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void debug_ftp(const char *msg, ...)
{
  #ifdef DEBUG_FTP
  va_list args;
  va_start(args, msg);
  fprintf(stdout, "🗩 ");
  vfprintf(stdout, msg, args);
  fprintf(stdout, "\n");
  va_end(args);
  #endif
}

void error(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  fprintf(stderr, "⚠ ");
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(-1);
}

void info(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  fprintf(stdout, "🛈 ");
  vfprintf(stdout, msg, args);
  fprintf(stdout, "\n");
  va_end(args);
}
