#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define SLEEP_INTERVAL (1000000U)

int main(void) {
    nvs_handle_t my_handle;
    int32_t temp = 0;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return err;
    }

    // Open NVS with RW access
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS!\n", err);
        return err;
    }

    // Main loop
    while (1) {
        // Read the device state from NVS
        err = nvs_get_i32(my_handle, STATE_KEY, &temp);
        if (err == ESP_OK) {
            // Print current state
            printf("Current state: %ld\n", temp);
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The state is not initialized yet!\n");
        } else {
            printf("Error (%d) reading!
", err);
        }

        // Update state
        temp = !temp; // Toggle state

        // Save the updated state
        err = nvs_set_i32(my_handle, STATE_KEY, temp);
        if (err != ESP_OK) {
            printf("Error (%d) writing!
", err);
        }

        // Commit value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing!
", err);
        }

        // Wait for a second
        xtimer_usleep(SLEEP_INTERVAL);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}