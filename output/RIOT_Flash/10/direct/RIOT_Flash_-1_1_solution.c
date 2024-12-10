#include <stdio.h>
#include <stdlib.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define DELAY 1  // Delay in seconds

void store_device_state(nvs_handle_t storage_handle, int state) {
    esp_err_t err = nvs_set_i32(storage_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!
");
    } else {
        err = nvs_commit(storage_handle);
        if (err != ESP_OK) {
            printf("Failed to commit to NVS!
");
        } else {
            printf("Device state stored successfully: %d
", state);
        }
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

    nvs_handle_t storage_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &storage_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return 1;
    }

    printf("Starting device state storage every second.\n");

    int device_state = 1;
    while (1) {
        store_device_state(storage_handle, device_state);
        // Toggle device state for demonstration
        device_state = device_state == 1 ? 0 : 1;
        xtimer_sleep(DELAY);
    }

    nvs_close(storage_handle);
    return 0;
}