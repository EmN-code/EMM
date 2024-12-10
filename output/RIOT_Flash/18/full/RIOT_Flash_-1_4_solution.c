#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        printf("NVS Flash Init failed. Retrying...\n");
        xtimer_sleep(1);
        err = nvs_flash_init();
    }
    printf("NVS Flash initialized successfully.\n");

    nvs_handle_t handle;
    // Open
    err = nvs_open("storage", NVS_READWRITE, &handle);
    while (err != ESP_OK) {
        printf("NVS Open failed. Retrying...\n");
        xtimer_sleep(1);
        err = nvs_open("storage", NVS_READWRITE, &handle);
    }
    printf("NVS Opened successfully.\n");

    int32_t device_state = 0;
    size_t state_size = sizeof(device_state);
    err = nvs_get_i32(handle, "state", &device_state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        device_state = 0;  // default state
        err = nvs_set_i32(handle, "state", device_state);
        nvs_commit(handle);
    }

    while(1) {
        device_state = 1 - device_state; // Toggle the state between 0 and 1
        err = nvs_set_i32(handle, "state", device_state);
        if(err != ESP_OK) {
            printf("Failed to set state in NVS.\n");
        } else {
            printf("Device state is now: %d\n", device_state);

            err = nvs_commit(handle);
            if(err != ESP_OK) {
                printf("Failed to commit state to NVS.\n");
            } else {
                printf("Device state committed successfully.\n");
            }
        }

        xtimer_sleep(1);
    }
    
    nvs_close(handle);
    return 0;
}