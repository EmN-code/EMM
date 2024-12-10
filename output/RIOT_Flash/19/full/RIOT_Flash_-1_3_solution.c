#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"
#define KEY_DEVICE_STATE "device_state"
#define ITERATION_DELAY (1U) // seconds

void setup_nvs(nvs_handle_t *handle) {
    while (1) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_OK) {
            printf("NVS Flash Initialized successfully\n");
            break;
        } else {
            printf("Failed to initialize NVS Flash: %d. Retrying...\n", err);
            xtimer_sleep(ITERATION_DELAY);
        }
    }

    while (1) {
        esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, handle);
        if (err == ESP_OK) {
            printf("NVS handle opened successfully\n");
            break;
        } else {
            printf("Failed to open NVS handle: %d. Retrying...\n", err);
            xtimer_sleep(ITERATION_DELAY);
        }
    }
}

void store_device_state(nvs_handle_t handle, int state) {
    while (1) {
        esp_err_t err = nvs_set_i32(handle, KEY_DEVICE_STATE, state);
        if (err == ESP_OK) {
            printf("Device state %d stored successfully\n", state);
            break;
        } else {
            printf("Failed to store device state: %d. Retrying...\n", err);
            xtimer_sleep(ITERATION_DELAY);
        }
    }
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    setup_nvs(&handle);

    int state = 0;
    while (1) {
        store_device_state(handle, state);
        state = !state; // Toggle state between 0 and 1
        xtimer_sleep(ITERATION_DELAY);
    }

    nvs_close(handle);
    return 0;
}