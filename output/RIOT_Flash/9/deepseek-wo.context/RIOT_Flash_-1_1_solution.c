
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) setting state!", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error (%s) committing state!", esp_err_to_name(err));
        } else {
            ESP_LOGI("NVS", "State %d stored successfully", state);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }

    int state = 0;

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Store the state in NVS
        store_state(my_handle, state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
