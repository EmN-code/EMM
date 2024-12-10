#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define INITIAL_STATE 0

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = INITIAL_STATE;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return err;
    }

    // Read the initial state from NVS
    err = nvs_get_i32(my_handle, STATE_KEY, &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("The value is not initialized yet!");
        state = INITIAL_STATE;
    } else if (err != ESP_OK) {
        printf("Error reading!");
    }

    while (1) {
        // Update state
        state = !state; // Toggle state between 0 and 1 for demo

        // Write updated state back to NVS
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS!");
        } else {
            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit NVS!");
            } else {
                printf("State updated to: %d\n", state);
            }
        }

        // Wait for one second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}