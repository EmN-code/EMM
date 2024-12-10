#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define INTERVAL (1U * US_PER_SEC) 

void update_device_state(nvs_handle_t my_handle, int state) {
    // Convert state integer to string
    char state_str[2];
    snprintf(state_str, sizeof(state_str), "%d", state);
    
    // Write the updated value back to NVS
    esp_err_t err = nvs_set_str(my_handle, NVS_KEY, state_str);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    } else {
        // Commit to ensure data is stored
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing!
", esp_err_to_name(err));
        } else {
            printf("Device state updated to: %d
", state);
        }
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!
", esp_err_to_name(err));
        return 1;
    }
    
    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS!
", esp_err_to_name(err));
        return 1;
    }

    int device_state = 0;

    while (1) {
        // Toggle device state between 0 and 1
        device_state = !device_state;

        // Update device state in NVS
        update_device_state(my_handle, device_state);

        // Wait for specified interval
        xtimer_sleep(INTERVAL);
    }

    // Close NVS handle (never reached in this case)
    nvs_close(my_handle);
    return 0;
}