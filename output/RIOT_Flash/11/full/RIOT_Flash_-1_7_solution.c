#include "ztimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS\n");
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        int32_t device_state = rand() % 2;  // Simulated device state as either 0 or 1

        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Failed to set i32 in NVS\n");
        } else {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit to NVS\n");
            }
        }

        printf("Device state '%d' written to NVS\n", device_state);
        sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}