
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error committing device state: %s\n", esp_err_to_name(err));
    }
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

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return 1;
    }

    // Main loop to store device state every second
    int state = 1; // Initial state
    while (1) {
        store_device_state(my_handle, state);
        state = (state == 1) ? 0 : 1; // Toggle state
        sleep(1); // Wait for 1 second
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
