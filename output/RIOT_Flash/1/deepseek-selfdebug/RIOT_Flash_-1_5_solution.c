
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state!\n", esp_err_to_name(err));
        }
    }
}

int32_t read_state(nvs_handle_t handle) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state!\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            int32_t current_state = read_state(my_handle);
            printf("Current state: %ld\n", current_state); // Use %ld for int32_t

            // Toggle state
            int32_t new_state = (current_state == 1) ? 0 : 1;
            store_state(my_handle, new_state);

            sleep(1); // Wait for 1 second
        }
        nvs_close(my_handle);
    }

    return 0;
}
