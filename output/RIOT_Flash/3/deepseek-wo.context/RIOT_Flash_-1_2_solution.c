
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static const char *TAG = "NVS_Example";

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error writing device state to NVS (%s)", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Error committing to NVS (%s)", esp_err_to_name(err));
        } else {
            ESP_LOGI(TAG, "Device state %d stored successfully", state);
        }
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle (%s)", esp_err_to_name(err));
        return err;
    }

    // Main loop to store device state every second
    int state = 1; // Initial state
    while (1) {
        store_device_state(my_handle, state);
        state = (state == 1) ? 0 : 1; // Toggle state
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
