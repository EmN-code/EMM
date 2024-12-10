
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "ztimer.h"
#include "stdio.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error committing NVS: %d\n", err);
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return 1;
    }

    // Main loop to update device state every second
    int32_t state = 1;
    while (1) {
        update_device_state(my_handle, state);
        printf("Device state updated to: %ld\n", (long)state);  // Corrected format specifier
        state = (state == 1) ? 0 : 1;  // Toggle state
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
