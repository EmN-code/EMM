
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Write state to NVS
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit writes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
