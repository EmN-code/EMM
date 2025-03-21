
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
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
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Simulate temperature update and store state
        uint8_t current_state = read_device_state(my_handle);
        printf("Current state: %d\n", current_state);

        // Toggle state
        current_state = (current_state == 0) ? 1 : 0;
        store_device_state(my_handle, current_state);

        // Wait for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
