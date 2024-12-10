#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_err.h>

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

volatile atomic_bool device_state = ATOMIC_VAR_INIT(false);

void save_to_nvs(int32_t state) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        err = nvs_set_i32(my_handle, STORAGE_KEY, state);
        if (err == ESP_OK) {
            err = nvs_commit(my_handle);
            printf((err != ESP_OK) ? "Failed to commit NVS: %s\n" : "State committed successfully\n", esp_err_to_name(err));
        } else {
            printf("Failed to write to NVS: %s\n", esp_err_to_name(err));
        }
        nvs_close(my_handle);
    } else {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
    }
}

void app_main() {
    while (1) {
        device_state = !device_state;  // Toggle device state
        save_to_nvs((int32_t)device_state);
        usleep(1000000); // Sleep for 1 second
    }
}
