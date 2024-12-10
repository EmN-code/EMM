
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int32_t state) {
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int32_t read_state(nvs_handle_t handle) {
    int32_t state = 0;
    nvs_get_i32(handle, NVS_KEY, &state);
    return state;
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

    // Open NVS
    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Read the current state from NVS
            int32_t current_state = read_state(my_handle);
            printf("Current state: %ld\n", current_state);

            // Toggle the state
            int32_t new_state = (current_state == 1) ? 0 : 1;
            printf("New state: %ld\n", new_state);

            // Store the new state in NVS
            store_state(my_handle, new_state);

            // Wait for 1 second
            sleep(1);
        }

        // Close NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
