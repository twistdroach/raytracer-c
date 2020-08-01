#ifndef UTILITIES_SIMPLE_LOGGER_H
#define UTILITIES_SIMPLE_LOGGER_H

typedef enum {
    ERROR,
    WARN,
    INFO
} LogLevel;

/**
 * Logs a "pretty" message to the terminal, this is simple & stupid
 * @param level A member of the LogLevel enum used to colorize terminal output
 * @param msg The null terminated string
 */
void SIMPLE_LOGGER_log(LogLevel level, const char* fmt, ...);

#endif //UTILITIES_SIMPLE_LOGGER_H
