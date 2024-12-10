
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DEVICE_STATE_KEY "device_state"
#define NVS_NAMESPACE "storage"

void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        } else {
            printf("State %d stored in NVS\n", state);
        }
    }
}

uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
    } else {
        printf("State %d read from NVS\n", state);
    }
    return state;
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        uint8_t state = read_state(my_handle);
        while (1) {
            state = !state; // Toggle state
            store_state(my_handle, state);
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
        }
        nvs_close(my_handle);
    }

    return 0;
}
