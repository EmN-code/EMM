#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <string.h>
#include <stdlib.h>

#define STORAGE_NAMESPACE "storage"

void app_main() {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return;
    }

    while (1) {
        // Read
        err = nvs_get_u8(my_handle, "device_state", &state);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("Value not set yet, defaulting to 0\n");
            state = 0;  // Default value
        } else if (err != ESP_OK) {
            printf("Error (%s) reading!
", esp_err_to_name(err));
        }

        // Toggle the state
        state = !state;

        // Write
        err = nvs_set_u8(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS!");
            nvs_close(my_handle);
            return;
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit to NVS!");
            nvs_close(my_handle);
            return;
        }

        printf("Device state updated to: %d\n", state);

        // Sleep for a second
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
}