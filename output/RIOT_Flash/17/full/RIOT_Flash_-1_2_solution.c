#include <xtimer.h>  // Required for sleep functionality in RIOT
#include <stdio.h>   // Standard I/O for printf
#include <nvs_flash.h>    // For NVS functionality

int main(void) {
    // Initialize NVS and handle errors
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!
", err);
        return err;
    }

    // Variable to store device state
    int32_t device_state = 0;

    // While loop simulates repeated process
    while (1) {
        // Write device state to NVS
        err = nvs_set_i32(my_handle, "state", device_state);
        if (err != ESP_OK) {
            printf("Error (%d) setting int value!
", err);
        } else {
            printf("Stored value: %ld
", (long)device_state);
        }

        // Update state (alternating between 0 and 1 for demo)
        device_state = 1 - device_state;

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing updates!
", err);
        }

        // Sleep for 1 second before next iteration
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return 0;
}