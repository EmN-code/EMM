#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

void store_device_state(nvs_handle_t nvs_handle) {
    static int state = 0; // Simulates the device state
    
    // Toggle state and store in NVS
    state = !state;
    nvs_set_i32(nvs_handle, "device_state", state);
    nvs_commit(nvs_handle);
    printf("Device state %d stored in NVS.\n", state);
}

int main() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS!\n");
        return -1;
    }

    // Open NVS handle
    nvs_handle_t nvs_handle;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle!\n");
        return -1;
    }

    // Infinite loop to update device state every second
    while (1) {
        store_device_state(nvs_handle);
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    // Close the NVS handle (not reachable in current design)
    nvs_close(nvs_handle);
    return 0;
}