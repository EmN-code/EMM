#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        nvs_flash_init();
    }

    while (1) {
        // Open NVS handle
        nvs_handle_t my_handle;
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            // Read current state
            int32_t state = 0; // default value
            err = nvs_get_i32(my_handle, "device_state", &state);
            switch (err) {
                case ESP_OK:
                    printf("Read device state = %d\n", state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet!\n");
                    break;
                default :
                    printf("Error (%s) reading!");
            }

            // Toggle the state
            state = !state;

            // Write updated state to NVS
            err = nvs_set_i32(my_handle, "device_state", state);
            printf((err != ESP_OK) ? "Failed to write state!\n" : "State written successfully!\n");

            // Commit written value.
            err = nvs_commit(my_handle);
            printf((err != ESP_OK) ? "Failed to commit!\n" : "Commit successful!\n");

            // Close
            nvs_close(my_handle);
        }

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}