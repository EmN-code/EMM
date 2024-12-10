
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DEVICE_STATE_KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state!\n", esp_err_to_name(err));
        } else {
            printf("State stored successfully!\n");
        }
    }
}

uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state!\n", esp_err_to_name(err));
    } else {
        printf("State read successfully: %d\n", state);
    }
    return state;
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Read the current state
        uint8_t current_state = read_state(my_handle);

        // Toggle the state
        uint8_t new_state = (current_state == 0) ? 1 : 0;

        // Store the new state
        store_state(my_handle, new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
