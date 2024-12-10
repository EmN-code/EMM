#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void save_state_nvs(int32_t state) {
    nvs_handle_t handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Write
        err = nvs_set_i32(handle, "state", state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        }

        // Commit
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(handle);
    }
}

int32_t read_state_nvs() {
    nvs_handle_t handle;
    esp_err_t err;

    int32_t state = 0; // Default state

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Read
        err = nvs_get_i32(handle, "state", &state);
        switch (err) {
            case ESP_OK:
                printf("Done. State = %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The state is not initialized yet!");
                break;
            default :
                printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(handle);
    }
    return state;
}

void app_main() {
    init_nvs();

    while (1) {
        int32_t state = read_state_nvs();
        state = 1 - state; // Toggle state
        save_state_nvs(state);

        // Simulate a delay of 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}