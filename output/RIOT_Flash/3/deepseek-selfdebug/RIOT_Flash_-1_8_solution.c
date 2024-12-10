
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_err.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error storing device state: %d\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing NVS: %d\n", err);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;
    int state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error erasing NVS flash: %d\n", err);
            return 1;
        }
        // Retry initialization
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return 1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return 1;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store the state in NVS
        store_device_state(handle, state);

        // Print the current state
        printf("Device state stored: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
