#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>
#include <xtimer.h>

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    while (err != ESP_OK) {
        printf("NVS flash init failed, retrying...\n");
        xtimer_sleep(1);
        err = nvs_flash_init();
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Failed to open NVS handle, retrying...\n");
        xtimer_sleep(1);
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
    }

    int32_t device_state = 0;
    while(1) {
        // Set device state
        err = nvs_set_i32(my_handle, "device_state", device_state);
        while (err != ESP_OK) {
            printf("Failed to set device state, retrying...\n");
            xtimer_sleep(1);
            err = nvs_set_i32(my_handle, "device_state", device_state);
        }

        // Commit the write operation
        err = nvs_commit(my_handle);
        while (err != ESP_OK) {
            printf("Failed to commit device state, retrying...\n");
            xtimer_sleep(1);
            err = nvs_commit(my_handle);
        }

        printf("Current device state: %d\n", device_state);

        // Toggle state for next iteration
        device_state = !device_state;

        // Wait for a second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}