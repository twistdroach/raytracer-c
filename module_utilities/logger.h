#ifndef UTILITIES_SIMPLE_LOGGER_H
#define UTILITIES_SIMPLE_LOGGER_H

typedef enum LOGGER_LEVEL { LOGGER_ERROR, LOGGER_WARN, LOGGER_INFO, LOGGER_DEBUG } LOGGER_LEVEL;

/**
 * Logs a "pretty" message to the terminal, this is simple & stupid
 * @param level A member of the LogLevel enum used to colorize terminal output
 * @param msg The null terminated string
 */
//void LOGGER_log(LOGGER_LEVEL level, const char *fmt, ...);
void LOGGER_log_with_file_line(LOGGER_LEVEL level, char *file, int line, const char *fmt, ...);

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
#define LOGGER_log(level, fmt, ...) LOGGER_log_with_file_line(level, __FILE__, __LINE__, fmt, ##__VA_ARGS__ )
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // UTILITIES_SIMPLE_LOGGER_H
