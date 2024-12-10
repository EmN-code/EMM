
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>

// Function to read the device state from NVS
int read_device_state(nvs_handle_t handle) {
    uint8_t state;
    esp_err_t err = nvs_get_u8(handle, "device_state", &state);
    if (err != ESP_OK) {
        printf("Error reading device state: %s\n", esp_err_to_name(err));
        return -1;
    }
    return state;
}

// Function to write the device state to NVS
esp_err_t write_device_state(nvs_handle_t handle, uint8_t state) {
    return nvs_set_u8(handle, "device_state", state);
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;

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
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Read the current device state
        int current_state = read_device_state(my_handle);
        if (current_state == -1) {
            printf("Failed to read device state\n");
            break;
        }

        // Toggle the state
        uint8_t new_state = (current_state == 0) ? 1 : 0;

        // Write the new state to NVS
        err = write_device_state(my_handle, new_state);
        if (err != ESP_OK) {
            printf("Error writing device state: %s\n", esp_err_to_name(err));
            break;
        }

        // Commit the changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing NVS changes: %s\n", esp_err_to_name(err));
            break;
        }

        // Print the new state
        printf("Device state updated to: %d\n", new_state);

        // Wait for 1 second before repeating
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
