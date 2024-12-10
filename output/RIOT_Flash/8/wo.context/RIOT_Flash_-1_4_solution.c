#include <xtimer.h>
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
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
        printf("Error (%s) setting value!\n", esp_err_to_name(err));
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing value!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

void init_nvs() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int main(void)
{
    init_nvs();

    int device_state = 0;

    while (1) {
        printf("Current device state: %d\n", device_state);
        store_device_state(device_state);

        // Toggle the state for demonstration purposes
        device_state = !device_state;

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}