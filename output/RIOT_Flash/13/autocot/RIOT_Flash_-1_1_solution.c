#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>
#include "esp_system.h"

void app_main(void) {
    // Initialize the NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        nvs_handle_t handle;
        int16_t temperature = 25; // Assume a temperature sensor value
        int32_t state = (temperature > 20) ? 1 : 0;

        // Open NVS handle
        err = nvs_open("storage", NVS_READWRITE, &handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            continue;
        }

        // Write the state into NVS
        err = nvs_set_i32(handle, "device_state", state);
        printf((err != ESP_OK) ? "Failed to set!\n" : "Set successful\n");

        // Commit the write
        err = nvs_commit(handle);
        printf((err != ESP_OK) ? "Failed to commit!\n" : "Commit successful\n");

        // Read the state back
        int32_t stored_state = 0; 
        err = nvs_get_i32(handle, "device_state", &stored_state);
        switch (err) {
            case ESP_OK:
                printf("Stored state = %d\n", (int) stored_state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Close NVS handle
        nvs_close(handle);

        // Wait for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}