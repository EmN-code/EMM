
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return 1;
    }

    // Open NVS
    nvs_handle_t handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    // Simulate continuous state update
    int state = 1;
    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store state in NVS
        store_state(handle, state);

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
