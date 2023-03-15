#include "../include/logger.h"

#include <stdio.h>  // for printf

enum level logLevel = NONE;

void logMsg(const enum level lvl, const char* msg) {
  // Print everything if the debug level is set
  if (logLevel == DEBUG) {
    printf("%s %s\n", levelString(lvl), msg);
    return;
  }
  // If the log level is one higher, we should not print debug statements
  if (lvl == DEBUG) {
    return;
  }
  // We only have INFO and ERROR left, if the logLevel is set to INFO both
  // should print
  if (logLevel == INFO) {
    printf("%s %s\n", levelString(lvl), msg);
    return;
  }
  if (lvl == INFO) {
    return;
  }
  // Only ERROR is left
  if (logLevel == ERROR) {
    printf("%s %s\n", levelString(lvl), msg);
  }
}
