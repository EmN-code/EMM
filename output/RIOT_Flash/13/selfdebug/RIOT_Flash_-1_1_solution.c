/*
 * The following code demonstrates how to use RIOT OS on an ESP32 to store and update device state using Non-Volatile Storage (NVS).
 * The state in this example will toggle between '1' and '0' every second, simulating a persistence mechanism.
 * We will use a simple loop in the main function to periodically update the NVS with some data.
 * The NVS functions are hypothetical equivalents in RIOT OS based on their ESP-IDF counterparts.
 */

#include "thread.h"
#include "periph/timer.h"
#include "nvs_flash.h" // Hypothetical RIOT equivalent to ESP-IDF's esp_partition.h
#include "nvs_handle.h" // Hypothetical header for NVS handle

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS\n", err);
        return 1;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return 1;
    }

    while (1) {
        // Store the state variable in NVS
        state = 1 - state; // toggle state
        err = nvs_set_u8(my_handle, "device_state", state);

        // Commit written value back to NVS
        if (err == ESP_OK) {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }
        } else {
            printf("Error (%d) setting NVS state value!\n", err);
        }

        printf("State set to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}