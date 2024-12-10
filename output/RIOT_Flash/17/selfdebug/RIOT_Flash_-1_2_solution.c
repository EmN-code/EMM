#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "state"
#define UPDATE_INTERVAL (1000000U) // 1 second in microseconds

void update_state(nvs_handle_t handle) {
    uint8_t state;
    // Read current state
    if (nvs_get_u8(handle, STATE_KEY, &state) != ESP_OK) {
        state = 0; // Default to 0 if not set
    }
    // Toggle state
    state = !state;
    printf("Updating state to: %d\n", state);
    // Write new state
    nvs_set_u8(handle, STATE_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated, erase and retry
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS storage
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Set up a timer to repeatedly update state
    while (1) {
        update_state(my_handle);
        xtimer_usleep(UPDATE_INTERVAL);
    }

    // Close NVS, not reached in this example, but good practice to close once done
    nvs_close(my_handle);
    return 0;
}