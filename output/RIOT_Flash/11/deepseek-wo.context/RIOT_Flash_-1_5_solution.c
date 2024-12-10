
#include <stdio.h>
#include <string.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing device state!\n", esp_err_to_name(err));
        } else {
            printf("Device state updated to %ld\n", (long)state);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;
    int32_t state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error (%s) erasing NVS flash!\n", esp_err_to_name(err));
            return err;
        }
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Update device state in NVS
        update_device_state(handle, state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
