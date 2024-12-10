
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <ztimer.h>
#include <esp_err.h>
#include <esp_log.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error setting state: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error committing state: %s", esp_err_to_name(err));
        }
    }
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int state = 1;

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
        ESP_LOGE("NVS", "Error opening NVS handle: %s", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store state in NVS
        store_state(my_handle, state);

        // Print current state
        printf("Current state: %d\n", state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
