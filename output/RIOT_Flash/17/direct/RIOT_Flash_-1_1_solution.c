#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define UPDATE_INTERVAL (1U * US_PER_SEC)

void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // If the NVS partition was truncated, erase it and try again
        nvs_flash_erase();
        nvs_flash_init();
    }
}

int get_device_state(void) {
    nvs_handle_t my_handle;
    int device_state = 0;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return 0;
    }

    // Read value from NVS
    size_t required_size = sizeof(device_state);
    nvs_get_blob(my_handle, DEVICE_STATE_KEY, &device_state, &required_size);

    // Close NVS handle
    nvs_close(my_handle);

    return device_state;
}

void set_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return;
    }

    // Write value to NVS
    nvs_set_blob(my_handle, DEVICE_STATE_KEY, &state, sizeof(state));
    nvs_commit(my_handle);

    // Close NVS handle
    nvs_close(my_handle);
}

int main(void) {
    printf("NVS Example: Storing state persistently\n");

    initialize_nvs();

    // Retrieve initial state
    int state = get_device_state();
    printf("Initial state: %d\n", state);

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Set new state in NVS
        set_device_state(state);

        // Print new state
        printf("Updated state: %d\n", state);

        // Wait for the next interval
        xtimer_sleep(UPDATE_INTERVAL);
    }

    return 0;
}