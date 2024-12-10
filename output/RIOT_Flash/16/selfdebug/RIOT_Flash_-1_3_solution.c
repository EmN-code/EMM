#include <xtimer.h>
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval of 1 second

void write_state_to_nvs(nvs_handle_t handle, const char *key, int32_t value) {
    // Write the integer value to NVS
    esp_err_t err = nvs_set_i32(handle, key, value);
    if (err != ESP_OK) {
        printf("Error (%d) saving state!\n", err);
    }
    // Commit written value to ensure it is saved
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing state!\n", err);
    }
    printf("State %d saved successfully.\n", value);
}

void timer_callback(void *arg) {
    static int32_t state = 0;
    nvs_handle_t handle;

    // Try to open NVS storage with RW permissions
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err == ESP_OK) {
        // Toggle state between 0 and 1
        state = !state;
        // Write state to NVS
        write_state_to_nvs(handle, "device_state", state);
        // Close NVS storage handle
        nvs_close(handle);
    } else {
        printf("Error (%d) opening NVS handle!\n", err);
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS flash.\n", err);
        return err;
    }

    // Initialize and set a timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        xtimer_sleep(TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}