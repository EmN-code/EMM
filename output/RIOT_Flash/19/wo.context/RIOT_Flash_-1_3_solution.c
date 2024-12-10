#include <stdio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_err.h>

static nvs_handle_t my_handle;

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

void store_state(int state) {
    esp_err_t err = nvs_set_i32(my_handle, "device_state", state);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);
}

void app_main() {
    init_nvs();
    int state = 0;

    while (1) {
        store_state(state);
        state = 1 - state; // Toggle state between 0 and 1
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}