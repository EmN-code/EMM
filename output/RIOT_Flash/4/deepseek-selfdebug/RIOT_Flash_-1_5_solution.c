
#include <stdio.h>
#include <unistd.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t device_state = 1; // Default state

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
        while (1) {
            // Read the current state from NVS
            err = nvs_get_i32(my_handle, NVS_KEY, &device_state);
            switch (err) {
                case ESP_OK:
                    printf("Device state: %d\n", device_state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("Device state not initialized yet!\n");
                    break;
                default:
                    printf("Error (%s) reading device state from NVS!\n", esp_err_to_name(err));
            }

            // Toggle the state
            device_state = (device_state == 1) ? 0 : 1;

            // Write the new state to NVS
            err = nvs_set_i32(my_handle, NVS_KEY, device_state);
            if (err != ESP_OK) {
                printf("Error (%s) writing device state to NVS!\n", esp_err_to_name(err));
            }

            // Commit the new state to NVS
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing device state to NVS!\n", esp_err_to_name(err));
            }

            // Wait for 1 second
            sleep(1);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}