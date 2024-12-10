#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "kernel.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void update_device_state(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err;
    int8_t device_state = 0;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_get_i8(nvs_handle, "device_state", &device_state);
        if (err == ESP_OK) {
            printf("Read device state: %d\n", device_state);
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The value is not initialized yet!");
        } else {
            printf("Error (%s) reading!");
        }

        device_state = !device_state;
        err = nvs_set_i8(nvs_handle, "device_state", device_state);
        printf((err != ESP_OK) ? "Failed to write!\n" : "Write successful\n");

        err = nvs_commit(nvs_handle);
        printf((err != ESP_OK) ? "Failed to commit!\n" : "Commit successful\n");

        nvs_close(nvs_handle);
    }
}

int main(void) {
    init_nvs();
    
    while (1) {
        update_device_state();
        xtimer_sleep(1);
    }

    return 0;
}