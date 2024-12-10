#include <stdio.h>
#include <xtimer.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define INTERVAL (1U * US_PER_SEC)

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %s\n", esp_err_to_name(err));
        return err;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return err;
    }

    int32_t device_state = 0;

    while (1) {
        // Update device state to 0 or 1, for simulation let's toggle it
        device_state = !device_state;
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error setting integer in NVS: %s\n", esp_err_to_name(err));
        }
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing changes in NVS: %s\n", esp_err_to_name(err));
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    nvs_close(my_handle);
    return 0;
}