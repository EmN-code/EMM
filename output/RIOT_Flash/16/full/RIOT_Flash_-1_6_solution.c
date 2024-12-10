#include <xtimer.h>
#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"
#define DEVICE_STATE_1 "1"
#define DEVICE_STATE_0 "0"

nvs_handle_t my_handle;

int main(void) {
    esp_err_t err;
    char state = '0'; // Initial state
    
    // Initialize NVS
    err = nvs_flash_init();
    while (err != ESP_OK) {
        printf("Failed to initialize NVS. Retrying in 1 second...\n");
        xtimer_sleep(1);
        err = nvs_flash_init();
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Failed to open NVS handle. Retrying in 1 second...\n");
        xtimer_sleep(1);
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }

    while (1) {
        // Toggle state
        state = (state == '0') ? '1' : '0';

        // Write state to NVS
        err = nvs_set_char(my_handle, STORAGE_KEY, state);
        if (err != ESP_OK) {
            printf("Failed to set state in NVS.\n");
            continue; // Try again
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes in NVS.\n");
            continue; // Try again
        }

        printf("Device state updated to: %c\n", state);
        xtimer_sleep(1); // wait for 1 second before repeating
    }

    // Close NVS handle (Technically unreachable in this loop)
    nvs_close(my_handle);
    return 0;
}