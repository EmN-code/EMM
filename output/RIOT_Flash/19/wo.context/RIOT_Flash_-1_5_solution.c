#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("NVS handle opened successfully\n");

        // Write
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%d) setting state in NVS!\n", err);
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing to NVS!\n", err);
        }

        // Close
        nvs_close(my_handle);
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    int device_state = 0;

    while (1) {
        store_device_state(device_state);
        printf("Stored device state: %d\n", device_state);

        // Toggle state for demonstration purposes
        device_state = !device_state;

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}