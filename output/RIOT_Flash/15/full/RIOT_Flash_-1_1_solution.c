#include <stdio.h>
#include <driver/gpio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"

int main(void) {
    nvs_flash_init();  // Initialize NVS
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error (%s) opening NVS handle! Retrying...\n", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }

    uint8_t device_state = 0;
    err = nvs_get_u8(my_handle, "state", &device_state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("Value not found, initializing to 0.\n");
        nvs_set_u8(my_handle, "state", device_state);
    }

    while (1) {
        device_state = !device_state;  // Toggle between 0 and 1
        err = nvs_set_u8(my_handle, "state", device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS\n", esp_err_to_name(err));
        }
        nvs_commit(my_handle);

        printf("Stored state: %d\n", device_state);
        xtimer_sleep(1);  // Sleep for a second
    }

    nvs_close(my_handle);  // Close NVS handle
    return 0;
}