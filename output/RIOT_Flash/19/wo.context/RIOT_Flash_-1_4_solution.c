#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    err = nvs_set_i32(nvs_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) setting state in NVS!\n", err);
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing changes in NVS!\n", err);
    }

    nvs_close(nvs_handle);
}

int main(void) {
    // Initialize the NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    int state = 0;
    while (1) {
        store_device_state(state);
        state = !state; // Toggle state between 0 and 1

        // Use a custom RIOT delay mechanism such as xtimer instead of vTaskDelay
        xtimer_sleep(1);
    }

    return 0;
}