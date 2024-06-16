#include "include.h"


const char *TAG = "app_main";


extern "C" void app_main(void) {
    // Crear tarea
    BaseType_t result = xTaskCreate(vUploadTask, "UploadTask", 2048, NULL, tskIDLE_PRIORITY, &xUploadTaskHandle);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Error al crear la tarea: UploadTask");
    }
    ESP_LOGI(TAG, "Tarea creada: UploadTask");

    // Crear tarea
    result = xTaskCreate(vSampleTask, "SampleTask", 2048, NULL, tskIDLE_PRIORITY, &xSampleTaskHandle);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Error al crear la tarea: SampleTask");
    }
    ESP_LOGI(TAG, "Tarea creada: SampleTask");
}