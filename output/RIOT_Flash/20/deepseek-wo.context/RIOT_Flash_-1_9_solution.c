
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

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

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        // Simulate device state (1 or 0)
        int32_t device_state = 1;

        while (1) {
            // Update device state in NVS
            err = nvs_set_i32(my_handle, NVS_KEY, device_state);
            if (err != ESP_OK) {
                printf("Error (%s) setting device state!\n", esp_err_to_name(err));
            } else {
                printf("Device state updated to %d\n", device_state);
            }

            // Commit the value to NVS
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            } else {
                printf("Device state committed to NVS\n");
            }

            // Toggle device state
            device_state = (device_state == 1) ? 0 : 1;

            // Wait for 1 second
            sleep(1);
        }

        // Close the NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
