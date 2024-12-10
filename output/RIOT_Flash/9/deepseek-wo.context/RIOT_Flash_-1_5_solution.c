
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "nvs.h"
#include "ztimer.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t device_state = 0;

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
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Toggle device state
            device_state = (device_state == 0) ? 1 : 0;

            // Store the device state in NVS
            err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
            if (err != ESP_OK) {
                printf("Error (%s) setting device state!\n", esp_err_to_name(err));
            } else {
                printf("Device state set to %ld\n", (long int)device_state);
            }

            // Commit the changes to NVS
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            } else {
                printf("NVS commit successful!\n");
            }

            // Wait for 1 second
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
