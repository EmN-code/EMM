#include "thread_flags.h"
#include "stdio.h"
#include "stdlib.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define NAMESPACE "storage"

void store_state(int state) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    // Open
    err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(nvs_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }

    // Commit written value.
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(nvs_handle);
}

int main(void) {
    int device_state = 0;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS flash!\n", esp_err_to_name(err));
        return 1;
    }

    while (1) {
        store_state(device_state);
        // Toggle state between 0 and 1
        device_state = !device_state;
        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}