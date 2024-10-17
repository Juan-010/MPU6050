#ifndef INCLUDE_H
#define INCLUDE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/i2c_types.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"



#include "cmdlib.h"

//Pinout
#define PIN_I2C_SDA GPIO_NUM_32
#define PIN_I2C_SCL GPIO_NUM_33

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