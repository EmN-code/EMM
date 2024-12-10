
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static const char *TAG = "NVS_Example";

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_blob(handle, DEVICE_STATE_KEY, &state, sizeof(state));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error writing device state: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Device state written successfully");
    }
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    size_t required_size = sizeof(state);
    esp_err_t err = nvs_get_blob(handle, DEVICE_STATE_KEY, &state, &required_size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error reading device state: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Device state read successfully: %d", state);
    }
    return state;
}

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return;
    }

    while (1) {
        uint8_t current_state = read_device_state(my_handle);
        current_state = (current_state == 1) ? 0 : 1; // Toggle state
        store_device_state(my_handle, current_state);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    nvs_close(my_handle);
}
