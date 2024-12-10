
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <ztimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t my_handle;
    int state = 1; // Initial state
    int err;

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
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("NVS handle opened successfully!\n");

        while (1) {
            // Toggle state
            state = (state == 1) ? 0 : 1;

            // Store state in NVS
            store_state(my_handle, state);

            // Print current state
            printf("Device state stored: %d\n", state);

            // Wait for 1 second
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        // Close NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
