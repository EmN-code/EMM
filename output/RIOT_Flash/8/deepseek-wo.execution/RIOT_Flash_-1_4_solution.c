
#include <stdlib.h>
#include <nvs_flash.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <string.h>

// Define the NVS handle as a global variable
nvs_handle_t my_handle;

// Function to initialize NVS
void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

// Function to store device state in NVS
void store_device_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    ESP_ERROR_CHECK(err);
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);
}

// Function to read device state from NVS
uint8_t read_device_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, "device_state", &state);
    if (err != ESP_OK) {
        state = 0; // Default state if not found
    }
    return state;
}

int main(void) {
    // Initialize NVS
    init_nvs();

    // Main loop
    while (1) {
        // Read current state
        uint8_t current_state = read_device_state();

        // Toggle state
        uint8_t new_state = (current_state == 1) ? 0 : 1;

        // Store new state
        store_device_state(new_state);

        // Print current state
        printf("Current Device State: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
