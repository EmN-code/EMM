#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, const int8_t state) {
    esp_err_t err = nvs_set_i8(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write device state to NVS\n");
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit to NVS\n");
    }
}

void app_main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS namespace
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return;
    }

    int8_t device_state = 0;

    while (1) {
        // Simulate reading temperature and deciding the next device state
        device_state = (device_state == 0) ? 1 : 0;  // Toggle state for demo purposes

        // Store state in NVS
        store_device_state(my_handle, device_state);

        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);
}