#include "include.h"

static const char *TAG = "uploadTask";

TaskHandle_t xUploadTaskHandle = NULL;

//create queue to send data to this task.
QueueHandle_t xUploadQueue = NULL;

void vUploadTask( void *pvParameters) 
{

    esp_err_t ret;

    // Setup UART
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    ret = uart_param_config(UART_NUM_0, &uart_config);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART configured.");
    ret = uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART pins set.");
    ret = uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART driver installed.");
    // Limpiar el buffer del UART
    uart_flush(UART_NUM_0);

    static Trama_t data;


    while (1)
    {
        if (xQueueReceive(xUploadQueue, (void *) &data, portMAX_DELAY) == pdTRUE)
        {
            #ifdef PRETTYPRINT
            printf("0x1b,ctr=%u,ax=%.2f,ay=%.2f,az=%.2f,gx=%.2f,gy=%.2f,gz=%.2f,t=%.2f\n", 
                    data.ctr, 
                    data.acce.acce_x, data.acce.acce_y, data.acce.acce_z, 
                    data.gyro.gyro_x, data.gyro.gyro_y, data.gyro.gyro_z, 
                    data.temp.temp);
            #else
            //resto 4 bytes para sacar el dato de temperatura.
            uart_write_bytes(UART_NUM_0, (const void *)&data.id, sizeof(uint8_t));
            uart_write_bytes(UART_NUM_0, (const void *)&data.ctr, sizeof(uint8_t));
            uart_write_bytes(UART_NUM_0, (const void *)&data.acce, 3*sizeof(float));
            uart_write_bytes(UART_NUM_0, (const void *)&data.gyro, 3*sizeof(float));
            #endif
        }
    }
}