
#include <stdarg.h>
#include <string.h>

#include "uart.h"


/* Size of log buffer output over UART and/or TCP/IP. */
#define LOG_BUFFER_SIZE   1024
#define LINE_FEED         "\r\n"    /* Linefeed for !Windows */

/* The buffer for log messages */
static char log_buffer[LOG_BUFFER_SIZE];


/*!
 * Write and format log messages to buffer.
 * @param format Format
 */
void log_printf(const char * format, ...) {

  va_list args;
  va_start(args, format);

  unsigned int len = strlen(log_buffer);
  vsnprintf(&log_buffer[len], LOG_BUFFER_SIZE - len, format, args);
  
  va_end(args);
}


/*!
 * Send log message over UART incl. LF at the end
 */
void log_uart_lf(void) {

  /* Send string over UART, without '\0' terminator */
  HAL_UART_Transmit(&hUART, (uint8_t *)log_buffer, strlen(log_buffer), 0xFFFF);

  /* Send line feed */
  HAL_UART_Transmit(&hUART, (uint8_t *)LINE_FEED, strlen(LINE_FEED), 0xFFFF);

  /* Clear log buffer for next messages */
  memset(log_buffer, 0, sizeof(log_buffer));
}


/*!
 * Send log message over UART without LF at the end
 */
void log_uart(void) {
    
  /* Send string over UART, without '\0' terminator */
  HAL_UART_Transmit(&hUART, (uint8_t *)log_buffer, strlen(log_buffer), 0xFFFF);
    
  /* Clear log buffer for next messages */
  memset(log_buffer, 0, sizeof(log_buffer));
}
