#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("Done\n");
        err = nvs_set_i32(my_handle, "device_state", state);
        printf((err != ESP_OK) ? "Failed to write!\n" : "Write successful\n");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed to commit!\n" : "Commit successful\n");
        nvs_close(my_handle);
    }
}

int main(void) {
    init_nvs();
    int device_state = 0;
    
    while (1) {
        store_device_state(device_state);
        device_state = !device_state; // toggle state
        xtimer_sleep(1);
    }
    return 0;
}