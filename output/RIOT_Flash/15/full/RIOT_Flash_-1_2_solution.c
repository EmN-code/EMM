#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void toggle_state(uint8_t *state) {
    *state = (*state == 0) ? 1 : 0;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    uint8_t state = 0;
    size_t required_size = sizeof(state);

    err = nvs_get_u8(my_handle, "device_state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error reading state from NVS!");
    }

    while (1) {
        toggle_state(&state);

        err = nvs_set_u8(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error writing state to NVS!");
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing state to NVS!");
        }

        printf("Device state updated to: %d\n", state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    nvs_close(my_handle);
    return 0;
}