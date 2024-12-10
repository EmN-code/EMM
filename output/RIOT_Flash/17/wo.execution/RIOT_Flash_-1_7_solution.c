#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "stdio.h"

static nvs_handle_t my_handle;

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_state_to_nvs(uint8_t state) {
    esp_err_t err;
    err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS!\n");
    }
}

void app_main() {
    init_nvs();

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) { 
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    uint8_t device_state = 0;
    while (1) {
        store_state_to_nvs(device_state);
        device_state = !device_state; // Toggle between 0 and 1
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
}