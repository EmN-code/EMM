
#include <stdio.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

int main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return 1;
    }

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    int32_t temperature = 0;
    while (1) {
        // Simulate temperature update
        temperature = (temperature == 0) ? 1 : 0;

        // Store the temperature in NVS
        err = nvs_set_i32(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            return 1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            return 1;
        }

        // Print the current temperature
        printf("Temperature updated to: %d\n", temperature);

        // Wait for 1 second before repeating
        sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
