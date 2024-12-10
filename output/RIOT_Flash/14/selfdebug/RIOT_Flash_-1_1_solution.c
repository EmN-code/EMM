#include <nvs_flash.h>
#include <nvs.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t nvs_handle;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    ESP_ERROR_CHECK(err);

    int32_t state = 0;

    while (1) {
        state = !state; // Toggle state between 0 and 1
        err = nvs_set_i32(nvs_handle, "state", state);
        ESP_ERROR_CHECK(err);

        err = nvs_commit(nvs_handle);
        ESP_ERROR_CHECK(err);

        printf("Stored state: %d\n", state);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    nvs_close(nvs_handle);
}