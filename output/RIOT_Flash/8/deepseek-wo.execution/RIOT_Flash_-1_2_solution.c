
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
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Simulate temperature update
            uint8_t current_state = read_device_state(my_handle);
            printf("Current device state: %d\n", current_state);

            // Toggle state
            uint8_t new_state = (current_state == 1) ? 0 : 1;
            store_device_state(my_handle, new_state);
            printf("Updated device state: %d\n", new_state);

            // Wait for 1 second
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
