#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

void retrieve_state(int *state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_get_i32(my_handle, "device_state", state);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The value is not initialized yet!\n");
            *state = 0; // Default value
        } else if (err != ESP_OK) {
            printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
    }
    nvs_close(my_handle);
}

void store_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS!\n");
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit to NVS!\n");
        }
    }
    nvs_close(my_handle);
}

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int main() {
    initialize_nvs();
    int state = 0;

    while (1) {
        retrieve_state(&state);
        printf("Current State: %d\n", state);

        // Toggle the state for demonstration
        state = !state;

        store_state(state);

        usleep(1000000); // Pause for a second
    }

    return 0;
}