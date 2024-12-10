
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Toggle state
        state = !state;

        // Store state in NVS
        err = nvs_set_u8(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS set failed: %s\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return -1;
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %s\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return -1;
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
