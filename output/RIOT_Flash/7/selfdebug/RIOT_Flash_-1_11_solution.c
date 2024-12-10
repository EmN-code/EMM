#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void save_state(const char* name, int8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    err = nvs_set_i8(my_handle, name, state);
    if (err != ESP_OK) return;
    err = nvs_commit(my_handle);
    if (err != ESP_OK) return;

    nvs_close(my_handle);
}

int8_t read_state(const char* name) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int8_t state = 0;
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) return -1;

    err = nvs_get_i8(my_handle, name, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return -1;

    nvs_close(my_handle);
    return state;
}

void app_task(void) {
    int8_t current_state;
    while (1) {
        // Assuming temperature-related logic which determines state
        current_state = (current_state == 1) ? 0 : 1; // Toggle state
        save_state("device_state", current_state);
        printf("State updated to %d\n", current_state);

        // Sleep for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Main function
int main(void) {
    initialize_nvs();
    xTaskCreate(&app_task, "app_task", 2048, NULL, 5, NULL);
    vTaskStartScheduler();
    return 0;
}