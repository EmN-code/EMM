#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void update_device_state(nvs_handle_t *nvs_handle) {
    int32_t state = 1; // The state to be saved
    esp_err_t err = nvs_set_i32(*nvs_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) saving state!\n", esp_err_to_name(err));
    } else {
        printf("State saved.\n");
    }
    nvs_commit(*nvs_handle);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t nvs_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened.\n");
    }

    while (1) {
        update_device_state(&nvs_handle);
        // Delay for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(nvs_handle);
    return 0;
}