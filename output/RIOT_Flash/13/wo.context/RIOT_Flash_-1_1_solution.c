#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "nvs.h"
#include "timer.h"

#define TIMER_INTERVAL (1000U) // Timer trigger every second

static void update_device_state(void *arg) {
    static nvs_handle_t my_handle;
    static uint8_t state = 0;
    esp_err_t err;

    // Open the NVS in read-write mode
    if (nvs_open("storage", NVS_READWRITE, &my_handle) != ESP_OK) {
        puts("Failed to open NVS handle");
        return;
    }

    // Toggle the device state
    state = !state;

    // Write the new state to NVS
    err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        puts("Failed to write to NVS");
    } else {
        puts("State updated in NVS");
    }

    // Commit the changes
    nvs_commit(my_handle);
    // Close the NVS handle
    nvs_close(my_handle);
}

int main(void) {
    // Initialize the NVS system
    if (nvs_flash_init() != ESP_OK) {
        puts("NVS flash init failed");
        return EXIT_FAILURE;
    }

    // Set a periodic timer for every 1 second
    timer_t timer;
    timer.callback = update_device_state;
    timer.arg = NULL;
    timer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Sleep to wait for the timer to trigger
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}