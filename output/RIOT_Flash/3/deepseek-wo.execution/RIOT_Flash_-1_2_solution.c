
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_blob(handle, DEVICE_STATE_KEY, &state, sizeof(state));
    nvs_commit(handle);
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    size_t required_size = sizeof(state);
    nvs_get_blob(handle, DEVICE_STATE_KEY, &state, &required_size);
    return state;
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Simulate temperature update and store state
        uint8_t current_state = read_device_state(my_handle);
        current_state = (current_state == 1) ? 0 : 1; // Toggle state
        store_device_state(my_handle, current_state);

        // Print current state
        printf("Current device state: %d\n", current_state);

        // Wait for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
