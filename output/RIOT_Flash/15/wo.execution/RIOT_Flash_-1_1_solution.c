#include <stdio.h>
#include <string.h>
#include "riot/threads.h"
#include "riot/xtimer.h"
#include "riot/nvs_flash/init.h"
#include "riot/nvs_flash.h"

// Constants
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define WRITE_INTERVAL (1000000) // 1 second in microseconds

// Thread stack for periodic state update
static char stack[THREAD_STACKSIZE_DEFAULT];

// Function to simulate a device state that changes every second
int simulate_device_state() {
    static int state = 0;
    state = !state; // Toggle state
    return state;
}

// Thread function to continually update device state in NVS
void *update_state_thread(void *arg) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!
");
        return NULL;
    }

    while (1) {
        // Simulate getting the device state
        int state = simulate_device_state();

        // Write state to NVS
        err = nvs_set_i32(handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Error writing to NVS!
");
        } else {
            nvs_commit(handle);
            printf("Device state updated to: %d\n", state);
        }

        // Wait for the specified interval
        xtimer_usleep(WRITE_INTERVAL);
    }

    nvs_close(handle);
    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS flash!
");
        return 1;
    }

    // Start the update state thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD, update_state_thread, NULL, "update_state");

    // Main loop does nothing, all action is in threads
    while (1) {
        thread_sleep();
    }

    return 0;
}