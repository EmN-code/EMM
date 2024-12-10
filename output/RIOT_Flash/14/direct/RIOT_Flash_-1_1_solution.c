#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define INTERVAL (1U * US_PER_SEC) // one-second interval

void update_device_state(nvs_handle_t my_handle, int state) {
    esp_err_t err;
    // Save the state
    err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%d) saving state!\n", err);
    }
    // Commit written value to stable storage
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing!
", err);
    }
}

void read_device_state(nvs_handle_t my_handle, int *state) {
    esp_err_t err = nvs_get_i32(my_handle, DEVICE_STATE_KEY, state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %d\n", *state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Device state not initialized yet!
");
            *state = 0; // default to 0 if not found
            break;
        default :
            printf("Error (%d) reading!
", err);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!
", err);
        return -1;
    }

    int state;

    // Read the stored state
    read_device_state(my_handle, &state);

    while (1) {
        // Toggle the device state
        state = (state == 1) ? 0 : 1;

        // Update the device state
        update_device_state(my_handle, state);

        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}