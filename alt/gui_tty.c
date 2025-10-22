


#if 1

#define BUFFER_LEN 32

#include <string.h>
#include <ctype.h>

/* Example includes. */
#include "FreeRTOS_CLI.h"

/* The CLI commands are defined in CLI-commands.c. */
void vRegisterSampleCLICommands( void );

static char input[BUFFER_LEN];
static char input_b[BUFFER_LEN];

static void TTY_thread (void const * arg)
{

  (void) arg;

  static unsigned int saved = 0;
  
  static uint8_t index = 0;
  char rx_char;

  static uint32_t printPrompt = 1;

  char *pcOutputString;
  portBASE_TYPE xReturned;

  pcOutputString = FreeRTOS_CLIGetOutputBuffer();

  /* Register two command line commands to show task stats and run time stats respectively. */
  vRegisterSampleCLICommands();

  while (1) {

    // Enable interrupt RX Not Empty Interrupt
    //__HAL_UART_ENABLE_IT(&hUART, UART_IT_RXNE);

    /* Do we have to print the prompt? */
    if(printPrompt) {
      //writef("> "); /* printf("\n"); */
      //vt100_puts("> ");
      
      printPrompt = 0;
      memset(input, 0, sizeof(input));
    }

    int const queue_rc = xQueueReceive(rx_char_queue, &rx_char, portMAX_DELAY);
    if (queue_rc == pdFALSE) {
        index = 0;
        continue; // Try again
    }

    // Process

    /* Check for simple line control characters */
    if(((rx_char == 010) || (rx_char == 0x7f)) && index) {
      /* User pressed backspace */
      writef("\010 \010");        /* Obliterate character */
      //vt100_putc('\b');
      index--;                    /* Then keep track of how many are left */
      input[index] = '\0';        /* Then remove it from the buffer */

    } else if(rx_char == '!') {   /* '!' repeats the last command */
      if(saved) {                 /* But only if we have something saved */
        strcpy(input, input_b);   /* Restore the command */
        //writef("HIS: %s",input);
        index = strlen(input);
        goto parseme;
      }
    } else if(isprint((unsigned int) rx_char)) {
      /* We are only going to save printable characters */
      if(index >= sizeof(input)) {

        /* We are out of space */
        writef("\x07");   /* Beep */
        continue;         /* TODO: Check this!*/

      } else {
        /* Store char in buffer */
        input[index] = rx_char;
        index++;
        /* Echo it back to the user */
        writef("%c", rx_char);
        //vt100_putc(rx_char);
      }
    } else if(rx_char == 0x1b) { /* ESC Key - forward to terminal only */
        //vt100_putc(rx_char);

    } else if(rx_char == '\r') {
      /* NULL Terminate anything we have received */
      input[index] = '\0';
      /* save current buffer in case we want to re-do the command */
      strcpy(input_b, input);
      saved = 1;

      /* The user pressed enter, parse the command */
    
    parseme:
      /* Send CR to console */
      writef("\r\n");
      //vt100_putc('\r'); 
      index = 0;
      
      #if 1
      do {
        xReturned = FreeRTOS_CLIProcessCommand( input, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );
        writef("%s", pcOutputString);
        //vt100_puts(pcOutputString);

      } while( xReturned != pdFALSE );

      //vt100_putc('\r');
      #endif

      printPrompt = 1;
    }

    /* We have a character to process */
    //writef("got:'%c' %d %d \r\n", c, c, count);

  }

}



#endif
