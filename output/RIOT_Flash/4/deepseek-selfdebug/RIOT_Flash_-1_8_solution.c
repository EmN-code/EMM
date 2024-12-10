
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

void update_temperature(nvs_handle_t handle, int temp) {
    esp_err_t err = nvs_set_i32(handle, TEMPERATURE_KEY, temp);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error setting temperature: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error committing to NVS: %s", esp_err_to_name(err));
        } else {
            ESP_LOGI("NVS", "Temperature updated to %d", temp);
        }
    }
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error opening NVS handle: %s", esp_err_to_name(err));
        return 1;
    }

    while (1) {
        int temp = (esp_random() % 100) < 50 ? 1 : 0; // Simulate temperature state
        update_temperature(my_handle, temp);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
