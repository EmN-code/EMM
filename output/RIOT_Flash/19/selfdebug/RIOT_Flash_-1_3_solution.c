#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>
#include <freertos/task.h>

#define STORAGE_NAMESPACE "storage"

void app_main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            int32_t device_state = 0;
            // Read
            err = nvs_get_i32(my_handle, "device_state", &device_state);
            if (err == ESP_OK) {
                printf("State = %d\n", device_state);
                // Toggle state
                device_state = !device_state;
            } else if (err == ESP_ERR_NVS_NOT_FOUND) {
                printf("The value is not initialized yet! Set to default 0.\n");
                device_state = 0;
            } else {
                printf("Error (%d) reading state from NVS!\n", err);
            }

            err = nvs_set_i32(my_handle, "device_state", device_state);
            if (err != ESP_OK) {
                printf("Error (%d) writing state to NVS!\n", err);
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing state to NVS!\n", err);
            }

            // Close
            nvs_close(my_handle);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}