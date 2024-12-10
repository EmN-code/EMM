#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void save_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

void toggle_device_state(void) {
    static int device_state = 0;  // Initial device state

    while (1) {
        save_device_state(device_state);

        // Toggle the state between 0 and 1
        device_state = !device_state;

        // Sleep for 1 second
        xtimer_sleep(1);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  toggle_device_state, NULL, "toggle_device_state");

    return 0;
}