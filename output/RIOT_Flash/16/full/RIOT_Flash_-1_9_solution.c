#include <stdio.h>
#include <nvs_flash.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define KEY_NAME "device_state"

void update_device_state(nvs_handle_t nvs_handle) {
    int32_t state = 0;
    esp_err_t err;

    // Read the current state
    err = nvs_get_i32(nvs_handle, KEY_NAME, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
    }

    // Toggle the state
    state = (state == 0) ? 1 : 0;

    // Write the new state
    err = nvs_set_i32(nvs_handle, KEY_NAME, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }

    // Display new state
    printf("Device state updated to %d\n", state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    nvs_handle_t my_handle;

    while (1) {
        err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("NVS open failed: %s\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        update_device_state(my_handle);

        // Close NVS handle
        nvs_close(my_handle);

        // Wait for a second
        xtimer_sleep(1);
    }

    return 0;
}