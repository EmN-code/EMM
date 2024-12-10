
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>

int main() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return 1;
    }

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return 1;
    }

    // Simulate temperature update every second
    while (1) {
        // Simulate temperature state (1 or 0)
        uint8_t temperature_state = (rand() % 2); // Randomly set to 1 or 0

        // Store the temperature state in NVS
        err = nvs_set_u8(my_handle, "temp_state", temperature_state);
        if (err != ESP_OK) {
            printf("NVS set failed: %s\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return 1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %s\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return 1;
        }

        // Print the current state
        printf("Temperature state updated to: %d\n", temperature_state);

        // Wait for 1 second before updating again
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
