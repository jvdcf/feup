/**
 * @file alarm.h
 * @brief Alarm abstraction used by the transmitter
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _ALARM_H_
#define _ALARM_H_

#include <stdbool.h>

// Initializes the alarm handler and sets the number of retries and timeout.
void alarmInit(int maxRetries, int timeout);
// Starts the alarm, i.e, calls alarm(TIMEOUT).
void alarmOn();
// Stops the alarm, i.e, calls alarm(0).
void alarmOff();
// Resets the alarm counter and stops the alarm.
void alarmReset();
// Returns true if the alarm is enabled.
bool alarmIsEnabled();
// Returns true if the maximum number of retries was reached.
bool alarmMaxRetriesReached();

#endif // _ALARM_H_