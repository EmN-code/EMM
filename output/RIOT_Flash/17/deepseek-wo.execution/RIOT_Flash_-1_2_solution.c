
#include <stdio.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_blob(handle, NVS_KEY, &state, sizeof(state));
    nvs_commit(handle);
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    size_t required_size = sizeof(state);
    nvs_get_blob(handle, NVS_KEY, &state, &required_size);
    return state;
}

int main() {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    // Main loop to store and read device state every second
    uint8_t state = 1; // Initial state
    while (1) {
        // Store the current state
        store_device_state(my_handle, state);

        // Read the stored state
        uint8_t stored_state = read_device_state(my_handle);
        printf("Stored device state: %d\n", stored_state);

        // Toggle the state
        state = state == 1 ? 0 : 1;

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
