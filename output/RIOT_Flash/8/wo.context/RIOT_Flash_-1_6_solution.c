#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_timer.h>
#include <string.h>
#include <stdlib.h>

#define STORAGE_NAMESPACE "storage"

void store_state(nvs_handle_t my_handle, int state) {
    esp_err_t err;
    
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }

    printf("State %d stored.
", state);
}

void app_main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated
      // and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      // Retry nvs_flash_init
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    printf("- NVS Initialized -\n");

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return;
    } else {
        printf("- NVS Handle Opened -\n");
    }

    int state = 0;

    while (1) {
        store_state(my_handle, state);
        state = 1 - state; // Toggle state between 0 and 1
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    nvs_close(my_handle);
}