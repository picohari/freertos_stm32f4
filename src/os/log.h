
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOG_H
#define __LOG_H

#include "config.h"


/* Basic logging functions */


/* Write and format log messages to buffer. */
void log_printf(const char * format, ...);

/* Send log message over UART incl. LF at the end */
void log_uart_lf(void);

/* Send log message over UART without LF at the end */
void log_uart(void);



/* Convenience macros for logging */

#define LOG_DEBUG(...) {log_printf(__VA_ARGS__);  \
                        log_uart_lf();            \
}

#define LOG_PRINTF(...) {log_printf(__VA_ARGS__); \
                         log_uart();              \
}


#endif /* __LOG_H */