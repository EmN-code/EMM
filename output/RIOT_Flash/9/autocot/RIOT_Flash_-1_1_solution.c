#include <xtimer.h>  // For setting up a timer to execute tasks periodically
#include <stdio.h>    // Standard input/output library for printing
#include <string.h>   // For string operations, including memset
#include "nvs.h"     // Include NVS (Non-Volatile Storage) handler
#include "nvs_flash.h" // Include flash memory API of the NVS

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "state"

static void temperature_monitor_thread(void)
{
    nvs_handle_t my_handle;
    int32_t state = 0;
    esp_err_t err;

    // Initialize the NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS storage
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("NVS handle opened successfully\n");

        // Read the initial state from NVS
        err = nvs_get_i32(my_handle, STATE_KEY, &state);

        switch (err) {
            case ESP_OK:
                printf("State = %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                state = 0; // default state
                break;
            default :
                printf("Error (%d) reading!\n", err);
        }

        // Start loop to update state every second
        while (1) {
            // Simulate temperature or state changes
            state = (state == 0) ? 1 : 0; // toggle state

            // Write updated state to NVS
            err = nvs_set_i32(my_handle, STATE_KEY, state);
            if (err != ESP_OK) {
                printf("Failed to write state to NVS!\n");
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit state in NVS!\n");
            }

            printf("Updated state = %d\n", state);

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS handle (not reached in this example)
        nvs_close(my_handle);
    }
}

int main(void)
{
    printf("Starting temperature monitoring with NVS storage...\n");
    temperature_monitor_thread();
    return 0;
}