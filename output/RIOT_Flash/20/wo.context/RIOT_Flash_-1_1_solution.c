#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>
#include <esp_sleep.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    int16_t temp = 0; // Simulate temperature
    int16_t hum = 0;  // Simulate humidity for completeness

    // Repeat every second
    while (1) {
        // Simulate a temperature read and update
        temp = (temp + 1) % 100; // Dummy operation to change temperature

        // Write temperature to NVS
        err = nvs_set_i16(my_handle, "temperature", temp);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }

        printf("Temperature updated in NVS: %d\n", temp);

        // Sleep for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}