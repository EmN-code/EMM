#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"
#include <stdio.h>

// Non-volatile storage handle
nvs_handle_t my_handle;

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(uint8_t state) {
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
    }

    // Write
    err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to set value!");
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes!");
    }

    // Close
    nvs_close(my_handle);
}

int main(void) {
    init_nvs();

    uint8_t state = 0;

    while (1) {
        state = !state; // Toggle state between 0 and 1
        store_device_state(state);
        printf("Stored device state: %d\n", state);
        xtimer_sleep(1); // Sleep for 1 second before next write
    }

    return 0;
}