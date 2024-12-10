#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <xtimer.h>

// Define storage namespace and keys
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, const char* state) {
    esp_err_t err = nvs_set_str(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set device state in NVS!");
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes to NVS!");
        }
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS!");
        return -1;
    }

    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle!");
        return -1;
    }

	int state_flag = 0;
    while (1) {
        // Toggle state between "1" and "0"
        const char* state = (state_flag % 2 == 0) ? "1" : "0";

        // Store state in NVS
        store_device_state(handle, state);

        // Simulate a repeated process every second
        xtimer_sleep(1);
        state_flag++;
    }

    nvs_close(handle);
    return 0;
}