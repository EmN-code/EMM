#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "nvs_example";

void update_device_state_in_nvs(int state) {
    nvs_handle_t my_handle;

    // Open NVS handle
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Updating state in NVS");
        // Write state to NVS
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write to NVS (%s)!", esp_err_to_name(err));
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to commit changes to NVS (%s)!", esp_err_to_name(err));
        }
        // Close handle state
        nvs_close(my_handle);
    }
}

void app_main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Initialize state
    int state = 0;

    // Main loop
    while (1) {
        update_device_state_in_nvs(state);
        state = !state;  // Toggle state between 0 and 1
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
    }
}