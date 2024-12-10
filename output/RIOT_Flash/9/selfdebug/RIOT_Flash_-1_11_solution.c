#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/task.h"

#define STORAGE_NAMESPACE "storage"
#define DELAY_SECONDS 1

void store_state_task(void *pvParameters) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            // Read the current state
            err = nvs_get_i32(my_handle, "state", &state);
            switch (err) {
                case ESP_OK:
                    printf("Current state = %d\n", state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet!\n");
                    state = 0;  // Default state
                    break;
                default :
                    printf("Error (%s) reading!");
            }

            // Toggle state
            state = !state;

            // Write the updated state back to NVS
            err = nvs_set_i32(my_handle, "state", state);
            printf((err != ESP_OK) ? "Failed to write to storage!\n" : "Successfully stored value\n");

            // Commit written value
            err = nvs_commit(my_handle);
            printf((err != ESP_OK) ? "Failed to commit to storage!\n" : "Successfully committed!\n");

            // Delay for a second
            vTaskDelay(DELAY_SECONDS * 1000 / portTICK_PERIOD_MS);
        }
        // Close NVS handle
        nvs_close(my_handle);
    }
}

void app_main() {
    xTaskCreate(&store_state_task, "store_state_task", 2048, NULL, 5, NULL);
    vTaskStartScheduler();
}