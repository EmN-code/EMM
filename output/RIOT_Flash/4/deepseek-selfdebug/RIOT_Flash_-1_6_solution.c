
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

void store_temperature(nvs_handle_t handle, int32_t temp) {
    esp_err_t err = nvs_set_i32(handle, TEMPERATURE_KEY, temp);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) setting temperature!", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error (%s) committing temperature!", esp_err_to_name(err));
        } else {
            ESP_LOGI("NVS", "Temperature stored successfully");
        }
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }

    while (1) {
        int32_t temp = rand() % 100; // Simulate temperature reading
        store_temperature(my_handle, temp);
        printf("Temperature updated to: %ld\n", (long)temp); // Use %ld for int32_t
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
