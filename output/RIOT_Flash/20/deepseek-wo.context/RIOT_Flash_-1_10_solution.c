
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t device_state = 0;
    int32_t temp = 0;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Simulate temperature update
            temp = (temp + 1) % 100;

            // Store temperature in NVS
            err = nvs_set_i32(my_handle, NVS_KEY, temp);
            if (err != ESP_OK) {
                printf("Error (%d) storing temperature!\n", err);
            } else {
                printf("Temperature stored: %ld\n", (long)temp);
            }

            // Commit the value to NVS
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
