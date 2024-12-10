#include <ztimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define UPDATE_INTERVAL (1U)  // One second

void update_device_state(nvs_handle_t handle) {
    int32_t state = 0;
    esp_err_t err;
    while (1) {
        // Read current state from NVS
        err = nvs_get_i32(handle, STATE_KEY, &state);
        if (err == ESP_OK) {
            printf("Current state: %d\n", state);
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The value is not initialized yet!");
            state = 0; // default state
        } else {
            printf("Error (%s) reading!
", esp_err_to_name(err));
        }

        // Toggle the state
        state = !state;

        // Write the updated state back to NVS
        while ((err = nvs_set_i32(handle, STATE_KEY, state)) != ESP_OK) {
            printf("Error (%s) writing! Retrying in 1s...\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        // Commit written value.
        while ((err = nvs_commit(handle)) != ESP_OK) {
            printf("Failed to commit state (%s). Retrying in 1s...\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        printf("State updated to: %d\n", state);
        ztimer_sleep(ZTIMER_MSEC, UPDATE_INTERVAL * 1000);
    }
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS (%s)\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS (%s)\n", esp_err_to_name(err));
        return -1;
    }

    // Start updating device state
    update_device_state(handle);

    // Close NVS handle
    nvs_close(handle);
    return 0;
}