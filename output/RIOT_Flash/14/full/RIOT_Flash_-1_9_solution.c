#include "esp_wifi.h"
#include "esp_system.h"
#include "xtimer.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"

void app_main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int8_t state = 0; // Initial state to store

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    while (1) {
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err == ESP_OK) {
            break;
        } else {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            xtimer_sleep(1);
        }
    }

    while (true) {
        // Write
        err = nvs_set_i8(my_handle, "state", state);
        if (err != ESP_OK) {
            printf("Failed to set value: (%s)\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit value: (%s)\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        printf("Stored state: %d\n", state);

        state = !state; //toggle state
        xtimer_sleep(1);
    }

    // Close
    nvs_close(my_handle);
}