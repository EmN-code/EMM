
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Simulate temperature update (1 or 0)
            int8_t device_state = (rand() % 2); // Randomly set state to 1 or 0

            // Write device state to NVS
            err = nvs_set_i8(my_handle, NVS_KEY, device_state);
            if (err != ESP_OK) {
                printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
            } else {
                printf("Device state %d written to NVS\n", device_state);
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            }

            // Wait for 1 second
            sleep(1);
        }

        // Close
        nvs_close(my_handle);
    }

    return 0;
}
