
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define KEY "device_state"

void toggle_state(nvs_handle_t handle, uint8_t *state) {
    *state = (*state == 0) ? 1 : 0;
    nvs_set_u8(handle, KEY, *state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    // Read initial state
    err = nvs_get_u8(my_handle, KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("NVS read failed: %d\n", err);
        nvs_close(my_handle);
        return -1;
    }

    while (1) {
        // Toggle state and store it
        toggle_state(my_handle, &state);

        // Print current state
        printf("Current state: %d\n", state);

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);
    return 0;
}
