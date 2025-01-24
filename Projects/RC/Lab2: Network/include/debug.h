/**
 * @file debug.h
 * @brief Auxiliary functions for printing debug messages
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

// Comment/uncomment these lines to disable/enable debug messages for each component
#define DEBUG_FTP

// Print an optional debug message for the FTP server messages
void debug_ftp(const char *msg, ...);
// Print a mandatory error message and exit the program
void error(const char *msg, ...);
// Print a mandatory info message
void info(const char *msg, ...);

#endif // _DEBUG_H_