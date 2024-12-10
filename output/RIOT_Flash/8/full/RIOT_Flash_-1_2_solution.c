#include <stdio.h>
#include <stdatomic.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>

#define STORAGE_NAMESPACE "storage"

void app_main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    int32_t device_state = 0; // starting state

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (1) {
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Write
        err = nvs_set_i32(my_handle, "state", device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Loop state and sleep
        device_state = device_state ? 0 : 1;
        printf("Stored device state: %d\n", device_state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close
    nvs_close(my_handle);
}