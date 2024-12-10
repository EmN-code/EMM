
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DEVICE_STATE_KEY "device_state"

void toggle_state(nvs_handle_t handle) {
    static uint8_t state = 0;
    state = !state;
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
    printf("Device state updated to: %d\n", state);
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        toggle_state(my_handle);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
