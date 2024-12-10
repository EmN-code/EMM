#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include "esp_system.h"

// Define sleeping time (1 second)
#define SLEEP_TIME (1U * 1000000U)

void store_device_state(nvs_handle_t my_handle, int device_state) {
    esp_err_t err;

    // Write the value to NVS
    err = nvs_set_i32(my_handle, "device_state", device_state);
    if (err != ESP_OK) printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));

    // Commit written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
}

int read_device_state(nvs_handle_t my_handle) {
    esp_err_t err;
    int device_state = 0;

    // Read the value from NVS
    err = nvs_get_i32(my_handle, "device_state", &device_state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %d\n", device_state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
    }
    return device_state;
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    nvs_handle_t my_handle;
    while(1) {
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if(err == ESP_OK) {
            break;
        } else {
            printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
            xtimer_sleep(1);
        }
    }

    int device_state = 0;
    while (1) {
        // Toggle device state between 0 and 1
        device_state = !device_state;
        store_device_state(my_handle, device_state);

        // Read device state from NVS
        int current_device_state = read_device_state(my_handle);

        // Sleep for 1 second
        xtimer_usleep(SLEEP_TIME);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}