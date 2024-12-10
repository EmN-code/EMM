// Required imports
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

// Function prototypes
void init_nvs(void);
void save_device_state(int state);
int read_device_state(void);

// Initialize NVS
void init_nvs(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      // Retry nvs_flash_init
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

// Save device state to NVS
void save_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    // Write
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) return;

    // Commit written value
    err = nvs_commit(my_handle);
    if (err != ESP_OK) return;

    // Close
    nvs_close(my_handle);
}

// Read device state from NVS
int read_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0; // Default state

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return state;

    // Read
    err = nvs_get_i32(my_handle, "state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return 0;

    // Close
    nvs_close(my_handle);

    return state;
}

int main(void) {
    // Initialize NVS
    init_nvs();

    while (1) {
        // Read the current state (for example, from a device sensor)
        int state = read_device_state();

        // Toggle state for demonstration
        state = (state == 0) ? 1 : 0;

        // Save new state to NVS
        save_device_state(state);

        // Print current state
        printf("Device state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }
    return 0;
}