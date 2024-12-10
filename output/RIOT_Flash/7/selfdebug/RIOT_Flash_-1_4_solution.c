#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define NVS_KEY "device_state"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Write
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%s) setting NVS value!\n", esp_err_to_name(err));
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing NVS value!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(my_handle);
    }
}

int main(void) {
    init_nvs();
    int device_state = 1;

    while (1) {
        device_state = 1 - device_state; // Toggle state between 0 and 1
        store_device_state(device_state);
        xtimer_sleep(1);
    }

    return 0;
}