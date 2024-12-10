#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define STORAGE_NAMESPACE "storage"

nvs_handle_t my_handle;

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void write_state_to_nvs(uint8_t state) {
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    err = nvs_set_u8(my_handle, "state", state);
    if (err != ESP_OK) return;

    err = nvs_commit(my_handle);
    if (err != ESP_OK) return;

    nvs_close(my_handle);
}

uint8_t read_state_from_nvs() {
    uint8_t state = 0;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_get_u8(my_handle, "state", &state);
        nvs_close(my_handle);
    }
    return state;
}

void app_main(void) {
    init_nvs();
    uint8_t state = read_state_from_nvs();
    while (1) {
        state = !state; // Toggle state between 1 and 0
        printf("State is %d\n", state);
        write_state_to_nvs(state);
        vTaskDelay(1000 / portTICK_RATE_MS); // Delay for 1 second
    }
}