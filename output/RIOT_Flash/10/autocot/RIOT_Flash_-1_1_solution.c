#include <stdio.h>
#include <stdint.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void write_device_state(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%d) writing!
", err);
    }
    else {
        printf("Device state %d written.
", state);
        nvs_commit(my_handle);
    }
}

uint8_t read_device_state(nvs_handle_t my_handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%d) reading!
", err);
    }
    return state;
}

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!
", err);
    }
}

int main(void) {
    init_nvs();
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!
", err);
        return -1;
    }

    while (1) {
        uint8_t current_state = read_device_state(my_handle);
        current_state = current_state ? 0 : 1; // Toggle state
        write_device_state(my_handle, current_state);
        xtimer_sleep(1); // Sleep for one second
    }

    nvs_close(my_handle); // Should normally never reach here
    return 0;
}