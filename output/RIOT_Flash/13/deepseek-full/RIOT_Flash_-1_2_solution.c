
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <unistd.h>
#include <stdatomic.h>
#include <nvs.h>
#include <stdio.h>
#include <time_units.h>
#include <nvs_flash.h>
#include <esp_system.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void toggle_state(nvs_handle_t handle) {
    int32_t state;
    esp_err_t err = nvs_get_i32(handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        state = 0; // Default state if not found
    }
    state = (state == 0) ? 1 : 0; // Toggle state
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int main() {
    esp_err_t err;
    nvs_handle_t handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        toggle_state(handle);
        sleep(1); // Wait for 1 second
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(handle);

    return 0;
}
