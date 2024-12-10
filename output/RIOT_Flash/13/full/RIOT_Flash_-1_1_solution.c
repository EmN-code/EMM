#include <stdlib.h>
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

// Define constants
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define TIMER_INTERVAL (1U * 1000000U)  // Interval in microseconds for a 1 second delay

void toggle_device_state(uint8_t *state) {
    *state = (*state == 0) ? 1 : 0;
    printf("Device state toggled to: %d\n", *state);
}

void store_device_state_in_nvs(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return;
    }

    err = nvs_set_u8(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state in NVS!\n");
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes!\n");
    }

    nvs_close(my_handle);
}

uint8_t restore_device_state_from_nvs() {
    nvs_handle_t my_handle;
    uint8_t state = 0;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return state;
    }

    err = nvs_get_u8(my_handle, STATE_KEY, &state);
    switch (err) {
        case ESP_OK:
            printf("Device state restored from NVS: %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Device state not found in NVS, using default state: 0\n");
            state = 0;
            break;
        default:
            printf("Error reading device state from NVS!\n");
            break;
    }

    nvs_close(my_handle);
    return state;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS!");
        return -1;
    }

    uint8_t device_state = restore_device_state_from_nvs();

    while (1) {
        toggle_device_state(&device_state);
        store_device_state_in_nvs(device_state);
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}