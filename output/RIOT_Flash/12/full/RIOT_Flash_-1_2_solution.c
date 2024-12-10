#include "nvs.h"
#include "string.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

void app_main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        int32_t current_device_state = 0;
        while (1) {
            // Try to read the device state
            err = nvs_get_i32(my_handle, "device_state", &current_device_state);
            if (err == ESP_ERR_NVS_NOT_FOUND) {
                printf("The value is not initialized yet!");
                current_device_state = 0;
            } else if (err != ESP_OK) {
                printf("Error (%s) reading!");
            }

            // Toggle the state
            current_device_state = current_device_state ? 0 : 1;

            // Write back the new state
            err = nvs_set_i32(my_handle, "device_state", current_device_state);
            if (err != ESP_OK) {
                printf("Failed to write the new state!");
            } else {
                printf("Device state set to %d\n", current_device_state);
            }

            // Commit the written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit!");
            }

            // Sleep for 1 second
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        // Close NVS handle
        nvs_close(my_handle);
    }
}

int main(void)
{
    app_main();
    return 0;
}