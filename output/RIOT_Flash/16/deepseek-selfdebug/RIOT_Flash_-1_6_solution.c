
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static const char *TAG = "NVS_Example";

void init_nvs(nvs_handle_t *handle) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, handle);
    ESP_ERROR_CHECK(err);
}

void store_device_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    ESP_ERROR_CHECK(err);
    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);
}

int32_t read_device_state(nvs_handle_t handle) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error reading device state from NVS");
    }
    return state;
}

int main(void) {
    nvs_handle_t my_handle;
    init_nvs(&my_handle);

    while (1) {
        int32_t current_state = read_device_state(my_handle);
        ESP_LOGI(TAG, "Current device state: %ld", current_state);

        // Simulate updating the device state (toggle between 1 and 0)
        int32_t new_state = (current_state == 1) ? 0 : 1;
        store_device_state(my_handle, new_state);

        // Wait for 1 second before repeating
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    nvs_close(my_handle);
    return 0;
}
