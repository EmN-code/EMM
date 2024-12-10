#include <stdio.h>
#include <stdint.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"

#define STORAGE_NAMESPACE "storage"

void update_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Simulate device state change
    static int32_t state = 0;
    state = (state + 1) % 2;

    // Write state to NVS
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
        return;
    }

    // Commit written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
        return;
    }

    // Close NVS handle
    nvs_close(my_handle);

    // Print state to console
    printf("Device state updated in NVS: %" PRId32 "\n", state);
}

int main(void) {
    while (1) {
        update_device_state();
        thread_sleep(1000);
    }
    return 0;
}