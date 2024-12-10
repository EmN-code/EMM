#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <stdlib.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>
#include <thread.h>

// Constants
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define STACKSIZE 1024

// Thread declaration
char stack[STACKSIZE];
kernel_pid_t thread_pid;

// Function to update and commit the state to NVS
void update_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s", esp_err_to_name(err));
        return;
    }

    // Open NVS as read/write
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Read the current state value (assume "0" if not set yet)
    err = nvs_get_u8(my_handle, STATE_KEY, &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        state = 0;
    } else if (err != ESP_OK) {
        printf("Error reading state from NVS: %s\n", esp_err_to_name(err));
        nvs_close(my_handle);
        return;
    }

    // Toggle state between "1" and "0" for this example purposes
    state = !state;

    // Write the new state value
    err = nvs_set_u8(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error writing state to NVS: %s\n", esp_err_to_name(err));
    }

    // Commit written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing state to NVS: %s\n", esp_err_to_name(err));
    }

    // Close NVS handle
    nvs_close(my_handle);

    // Debug output
    printf("State updated to: %d\n", state);
}

// Thread function to periodically update the NVS stored state
void *state_update_thread(void *arg) {
    while (1) {
        update_state();
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

// Main application entry point
int main(void) {
    // Create a thread to update the state
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, state_update_thread, NULL, "state_thread");
    if (thread_pid <= 0) {
        printf("Failed to create state update thread.\n");
        return 1;
    }
    // Keep the main process alive
    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}
