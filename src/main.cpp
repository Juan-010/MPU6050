#include "include.h"


const char *TAG = "app_main";


extern "C" void app_main(void) {
    // Crear cola de mensajes
    xUploadQueue = xQueueCreate(16, sizeof(Trama_t));
    if (xUploadQueue == NULL)
    {
        ESP_LOGE(TAG, "Error al crear la cola.");
    }

    // Crear tarea
    BaseType_t result = xTaskCreate(vUploadTask, "UploadTask", 4096, NULL, tskIDLE_PRIORITY, &xUploadTaskHandle);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Error al crear la tarea: UploadTask");
    }
    ESP_LOGI(TAG, "Tarea creada: UploadTask");

    // Crear tarea
    result = xTaskCreate(vSampleTask, "SampleTask", 4096, NULL, 1, &xSampleTaskHandle);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Error al crear la tarea: SampleTask");
    }
    ESP_LOGI(TAG, "Tarea creada: SampleTask");
}