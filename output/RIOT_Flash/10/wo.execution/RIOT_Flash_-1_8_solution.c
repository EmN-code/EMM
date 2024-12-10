#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

void periodic_storage_update(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS\n");
        return;
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Infinite loop to update NVS every second
    while (1) {
        int32_t state = 1; // Example state to store
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err == ESP_OK) {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit state to NVS\n");
            }
        }
        else {
            printf("Failed to write state to NVS\n");
        }

        // Sleep for 1 second
        sleep(1); // Simulate waiting for 1 second
    }

    // Close NVS handle (unreachable in current loop)
    nvs_close(my_handle);
}

int main(void) {
    periodic_storage_update();
    return 0;
}