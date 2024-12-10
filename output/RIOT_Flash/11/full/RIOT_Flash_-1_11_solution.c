#include "ztimer.h"
#include "stdbool.h"
#include "system.h"
#include "stdio.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "esp_wifi.h"

nvs_handle_t my_handle;

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      nvs_flash_erase();
      // Retry nvs_flash_init
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

int store_device_state(int32_t state) {
    esp_err_t err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
        return err;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

int main() {
    init_nvs();

    int32_t state = 0;

    while (1) {
        state = !state; // Toggle state between 1 and 0
        esp_err_t result = store_device_state(state);
        if (result == ESP_OK) {
            printf("Device state stored: %d\n", state);
        } else {
            printf("Failed to store device state\n");
        }
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}