
#include <stdio.h>
#include <unistd.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

int main(void)
{
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t temperature = 0;

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
            // Read the current temperature from NVS
            err = nvs_get_i32(my_handle, TEMPERATURE_KEY, &temperature);
            switch (err) {
                case ESP_OK:
                    printf("Temperature: %d\n", temperature);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("Temperature key not found, initializing to 0\n");
                    temperature = 0;
                    break;
                default:
                    printf("Error (%s) reading temperature!\n", esp_err_to_name(err));
            }

            // Toggle the temperature value
            temperature = (temperature == 0) ? 1 : 0;

            // Write the new temperature value to NVS
            err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temperature);
            if (err != ESP_OK) {
                printf("Error (%s) writing temperature!\n", esp_err_to_name(err));
            }

            // Commit the changes to NVS
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing changes!\n", esp_err_to_name(err));
            }

            // Wait for 1 second
            sleep(1);
        }

        // Close the NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
