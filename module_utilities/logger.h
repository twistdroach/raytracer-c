#ifndef UTILITIES_SIMPLE_LOGGER_H
#define UTILITIES_SIMPLE_LOGGER_H

typedef enum LOGGER_LEVEL {
    LOGGER_ERROR,
    LOGGER_WARN,
    LOGGER_INFO,
    LOGGER_DEBUG
} LOGGER_LEVEL;

/**
 * Logs a "pretty" message to the terminal, this is simple & stupid
 * @param level A member of the LogLevel enum used to colorize terminal output
 * @param msg The null terminated string
 */
void LOGGER_log(LOGGER_LEVEL level, const char* fmt, ...);

#endif //UTILITIES_SIMPLE_LOGGER_H
