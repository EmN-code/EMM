
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err;
    err = nvs_set_blob(handle, KEY, &state, sizeof(state));
    if (err != ESP_OK) {
        printf("Error writing to NVS (%s)\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS (%s)\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(nvs_handle_t handle) {
    esp_err_t err;
    uint8_t state = 0;
    size_t required_size = sizeof(state);
    err = nvs_get_blob(handle, KEY, &state, &required_size);
    if (err != ESP_OK) {
        printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
    }
    return state;
}

int main() {
    esp_err_t err;
    nvs_handle_t my_handle;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS (%s)\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Read the current state
        state = read_state(my_handle);
        printf("Current state: %d\n", state);

        // Toggle the state
        state = (state == 0) ? 1 : 0;

        // Store the new state
        store_state(my_handle, state);

        // Wait for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
