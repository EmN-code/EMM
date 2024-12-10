
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define DEVICE_STATE_KEY "device_state"
#define TEMPERATURE_KEY "temperature"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t device_state = 1; // Default state
    int32_t temperature = 25; // Default temperature
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
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Toggle device state
        device_state = (device_state == 1) ? 0 : 1;

        // Update temperature (simulated)
        temperature += 1;

        // Store device state in NVS
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("Failed to set device state: %s\n", esp_err_to_name(err));
        }

        // Store temperature in NVS
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temperature);
        if (err != ESP_OK) {
            printf("Failed to set temperature: %s\n", esp_err_to_name(err));
        }

        // Commit changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %s\n", esp_err_to_name(err));
        }

        // Print current state and temperature
        printf("Device State: %d, Temperature: %d\n", device_state, temperature);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
