
#include <stdio.h>
#include <unistd.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

void store_temperature(nvs_handle_t handle, int temp) {
    esp_err_t err = nvs_set_i32(handle, TEMPERATURE_KEY, temp);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %s\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS: %s\n", esp_err_to_name(err));
        } else {
            printf("Temperature stored: %d\n", temp);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error erasing NVS: %s\n", esp_err_to_name(err));
            return err;
        }
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return err;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS: %s\n", esp_err_to_name(err));
        return err;
    }

    // Simulate temperature update every second
    int temp = 0;
    while (1) {
        store_temperature(handle, temp);
        temp = (temp == 0) ? 1 : 0; // Toggle temperature
        sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
