#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

// Define the NVS key and namespace
#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

// Function to read the device state from NVS
int read_device_state(nvs_handle_t my_handle, int *state) {
    size_t required_size;
    esp_err_t err = nvs_get_blob(my_handle, DEVICE_STATE_KEY, state, &required_size);
    if (err == ESP_OK && required_size == sizeof(int)) {
        return 0;
    }
    return -1;
}

// Function to write the device state to NVS
int write_device_state(nvs_handle_t my_handle, int state) {
    esp_err_t err = nvs_set_blob(my_handle, DEVICE_STATE_KEY, &state, sizeof(int));
    if (err != ESP_OK) {
        return -1;
    }
    return nvs_commit(my_handle);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS!\n");
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return -1;
    }

    int device_state = 0;

    // Main loop to toggle device state and save to NVS every second
    while (1) {
        // Toggle device state
        device_state = !device_state;

        // Write the device state to NVS
        if (write_device_state(my_handle, device_state) != 0) {
            printf("Error writing device state to NVS!\n");
        } else {
            printf("Device state saved: %d\n", device_state);
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle (unreachable code in this example)
    nvs_close(my_handle);

    return 0;
}