#include "nvs_flash.h"
#include "nvs.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "periph/gpio.h"

#define STORAGE_NAMESPACE "storage"

void check_esp_err(esp_err_t err) {
    if (err != ESP_OK) {
        fprintf(stderr, "Error: %s\n", esp_err_to_name(err));
        exit(EXIT_FAILURE);
    }
}

void save_state(nvs_handle_t handle, const char *key, const char *value) {
    esp_err_t err = nvs_set_str(handle, key, value);
    check_esp_err(err);
    err = nvs_commit(handle);
    check_esp_err(err);
}

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        check_esp_err(nvs_flash_erase());
        err = nvs_flash_init();
    }
    check_esp_err(err);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    initialize_nvs();

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    check_esp_err(err);

    while (1) {
        char current_state[2]; // To store "1" or "0"
        size_t required_size = sizeof(current_state);

        printf("Reading state from NVS...\n");
        err = nvs_get_str(my_handle, "device_state", current_state, &required_size);

        if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("State not set yet, setting default to '0'\n");
            strcpy(current_state, "0");
        } else {
            check_esp_err(err);
        }

        printf("Current state: %s\n", current_state);

        // Toggle state
        if (strcmp(current_state, "0") == 0) {
            strcpy(current_state, "1");
        } else {
            strcpy(current_state, "0");
        }

        printf("Saving new state: %s\n", current_state);
        save_state(my_handle, "device_state", current_state);

        // Sleep for a second
        for (volatile int i = 0; i < 1000000; ++i);
    }

    nvs_close(my_handle);
    return 0;
}
