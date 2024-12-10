#include <stdio.h>
#include <string.h> // Import for strcpy function
#include "nvs_flash.h" // ESP-IDF NVS import
#include "nvs.h"
#include "esp_system.h"

#define STORAGE_NAMESPACE "storage"

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    int32_t state = 0; // Variable to store the state

    while (1) {
        // Toggle the state between 0 and 1
        state = !state;

        // Write the updated state to NVS
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
        }

        // Commit the written value to NVS to ensure it's saved
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing to NVS!\n", err);
        }

        printf("Device state updated to %d\n", state);

        // Wait for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close the handle when done with NVS
    nvs_close(my_handle);
}