#include <stdio.h>
#include <freertos/task.h>
#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

void app_main(void) {
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
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    uint8_t device_state = 0;
    size_t required_size = sizeof(device_state);

    while (1) {
        // Simulate reading the device state
        device_state = !device_state;  // Toggle state between 0 and 1

        // Write to NVS
        err = nvs_set_u8(my_handle, "device_state", device_state);
        ESP_ERROR_CHECK(err);

        // Commit written value.
        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        printf("Stored device state: %d\n", device_state);

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Close
    nvs_close(my_handle);
}