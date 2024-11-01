#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include "driver/uart.h"

#define UART_PORT_NUM UART_NUM_0   // UART port number
#define UART_BAUD_RATE 115200      // UART baud rate
#define UART_TX_PIN 1             // UART TX pin
#define UART_RX_PIN 3            // UART RX pin

esp_err_t uart_init(void); 

#endif