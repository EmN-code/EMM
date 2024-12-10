
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(nvs_handle_t handle, int state) {
    int32_t state_to_store = state;
    nvs_set_i32(handle, NVS_KEY, state_to_store);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err;
    int state = 0;

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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Read the current state from NVS
        int32_t stored_state;
        err = nvs_get_i32(handle, NVS_KEY, &stored_state);
        switch (err) {
            case ESP_OK:
                state = stored_state;
                printf("State read from NVS: %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("State not found in NVS, initializing to 0\n");
                break;
            default:
                printf("Error (%s) reading state from NVS!\n", esp_err_to_name(err));
        }

        // Main loop to update and store state every second
        while (1) {
            // Toggle the state
            state = (state == 0) ? 1 : 0;

            // Store the updated state in NVS
            store_state(handle, state);

            // Print the current state
            printf("Updated state: %d\n", state);

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS
        nvs_close(handle);
    }

    return 0;
}
