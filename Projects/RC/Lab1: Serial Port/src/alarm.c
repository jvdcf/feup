#include "../include/alarm.h"
#include "../include/debug.h"
#include <signal.h>
#include <unistd.h>

// The maximum number of retries before giving up.
int maxRetries;
// Seconds to wait before the alarm triggers.
int timeout;
// If the alarm is enabled or not.
bool alarmEnabled = false;
// The number of times the alarm has been triggered by the SIGALRM signal.
int alarmCount = 0;

void alarmHandler(int signal) {
  alarmEnabled = false;
  alarmCount++;
  statisticsTimeout();
  debug_am("Triggered alarm nº%d", alarmCount);
}

// =====================================================================

void alarmInit(int mr, int to) {
  (void)signal(SIGALRM, alarmHandler);
  maxRetries = mr;
  timeout = to;
}

void alarmOn() {
  alarm(timeout);
  alarmEnabled = true;
}

void alarmOff() {
  alarm(0);
  alarmEnabled = false;
}

void alarmReset() {
  alarmCount = 0;
  alarmOff();
}

bool alarmIsEnabled() { return alarmEnabled; }

bool alarmMaxRetriesReached() { return alarmCount >= maxRetries; }
