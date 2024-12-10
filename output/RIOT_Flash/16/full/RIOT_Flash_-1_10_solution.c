#include "nvs_flash.h"
#include "nvs.h"
#include "stdio.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

nvs_handle_t my_handle;

void initialize_nvs(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int32_t state) {
    esp_err_t err;
    while (1) {
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!
", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Device state successfully stored.
");
            break;
        }
    }
    nvs_commit(my_handle);
}

void app_main(void) {
    initialize_nvs();
    int32_t state = 0;

    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return;
    }
    
    while (1) {
        state = (state == 0) ? 1 : 0; // Toggle state
        store_device_state(state);
        xtimer_sleep(1); // Wait for 1 second before toggling again
    }

    nvs_close(my_handle);
}