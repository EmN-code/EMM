
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define TEMPERATURE_KEY "temperature"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t device_state = 0;
    int32_t temperature = 25; // Example temperature value
    int err;

    // Initialize NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    // Erase NVS flash (optional, for demonstration purposes)
    err = nvs_erase_all(my_handle);
    if (err != NVS_OK) {
        printf("NVS erase failed: %d\n", err);
        nvs_close(my_handle);
        return -1;
    }

    // Commit the erase operation
    err = nvs_commit(my_handle);
    if (err != NVS_OK) {
        printf("NVS commit failed: %d\n", err);
        nvs_close(my_handle);
        return -1;
    }

    while (1) {
        // Toggle device state
        device_state = (device_state == 0) ? 1 : 0;

        // Update temperature (simulated)
        temperature += 1; // Simulate temperature increase

        // Store device state in NVS
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
        if (err != NVS_OK) {
            printf("NVS set device state failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Store temperature in NVS
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temperature);
        if (err != NVS_OK) {
            printf("NVS set temperature failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit the write operation
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print current state and temperature
        printf("Device State: %ld, Temperature: %ld\n", (long)device_state, (long)temperature);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
