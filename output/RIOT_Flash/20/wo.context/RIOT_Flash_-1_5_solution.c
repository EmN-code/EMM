#include "nvs_flash.h"  // Include for NVS functions
#include "nvs.h"        // Include for NVS handle
#include <stdio.h>       // Standard Input and Output
#include "xtimer.h"     // Include for xtimer functions

#define STORAGE_NAMESPACE "storage"

void check_nvs_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
        fflush(stdout);
    }
}

void save_device_state(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    check_nvs_error(err);
    err = nvs_commit(my_handle);
    check_nvs_error(err);
    printf("Device state saved: %d\n", state);
    fflush(stdout);
}

uint8_t read_device_state(nvs_handle_t my_handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, "device_state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not found, initializing to 0\n");
        state = 0;
        save_device_state(my_handle, state);
    }
    else {
        check_nvs_error(err);
    }
    printf("Read device state: %d\n", state);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    check_nvs_error(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    check_nvs_error(err);

    uint8_t current_state = read_device_state(my_handle);

    while (1) {
        current_state = !current_state;  // Toggle state between 0 and 1
        save_device_state(my_handle, current_state);
        xtimer_sleep(1);  // Wait for 1 second
    }

    nvs_close(my_handle);
    return 0;
}