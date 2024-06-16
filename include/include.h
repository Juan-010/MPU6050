#ifndef INCLUDE_H
#define INCLUDE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "cmdlib.h"

//Pinout
#define PIN_NUM_CLK  18
#define PIN_NUM_MOSI 23
#define PIN_NUM_CS   5
#define PIN_NUM_MISO 19

#define TX_PIN (UART_PIN_NO_CHANGE)
#define RX_PIN (UART_PIN_NO_CHANGE)

//Task: uploadTask
extern TaskHandle_t xUploadTaskHandle;
void vUploadTask( void *pvParameters );
extern QueueHandle_t xUploadQueue;

//Task: sampleTask
extern TaskHandle_t xSampleTaskHandle;
void vSampleTask( void *pvParameters );

#endif