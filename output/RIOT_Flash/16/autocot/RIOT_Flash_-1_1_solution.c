// Description:
// This code is designed for the ESP32 and uses RIOT OS to store a state ('1' or '0') in the Non-Volatile Storage (NVS). The state is updated every second, simulating a temperature read and storage process.
//
// The main parts of the code include initialization of NVS, reading the current state, updating it, and writing it back every second.

#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"
#include "stdio.h"

#define STORAGE_NAMESPACE "storage"
#define UPDATE_INTERVAL (1U * US_PER_SEC) // 1 second

void handle_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    handle_error(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    handle_error(err);

    // read previously saved state
    uint8_t device_state = 0;
    err = nvs_get_u8(my_handle, "state", &device_state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        // If no value is found, initialize with '0'
        device_state = 0;
    } else {
        handle_error(err);
    }

    while (1) {
        printf("Current state: %d\n", device_state);
        
        // Update the device state
        device_state = (device_state == 0) ? 1 : 0;

        err = nvs_set_u8(my_handle, "state", device_state);
        handle_error(err);

        err = nvs_commit(my_handle);
        handle_error(err);

        printf("Updated state to: %d\n", device_state);

        // Wait for one second
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}