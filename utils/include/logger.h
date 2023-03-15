#ifndef UTILS_INCLUDE_LOGGER_H_
#define UTILS_INCLUDE_LOGGER_H_

enum level { DEBUG, INFO, ERROR, NONE };
static inline const char *levelString(enum level lvl) {
  static const char *strings[] = {"[DEBUG]", "[INFO]", "[ERROR]"};
  return strings[lvl];
}

extern enum level logLevel;

void logMsg(const enum level lvl, const char *msg);

#endif  // UTILS_INCLUDE_LOGGER_H_
