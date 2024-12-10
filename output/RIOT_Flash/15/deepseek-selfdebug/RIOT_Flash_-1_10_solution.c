
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int device_state = 1; // Initial state

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
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Toggle device state
            device_state = (device_state == 1) ? 0 : 1;

            // Store the new state in NVS
            store_device_state(my_handle, device_state);

            // Print the current state
            printf("Device state updated to: %d\n", device_state);

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
