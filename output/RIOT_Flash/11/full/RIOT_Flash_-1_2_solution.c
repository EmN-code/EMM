#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Function to write the device state to NVS
eps_err_t write_device_state(nvs_handle_t handle, const char *state) {
    eps_err_t err = nvs_set_str(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }
    return err;
}

// Main function where everything is set up
int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle_t my_handle;

    // Open NVS handle
    while (1) {
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
            sleep(1);
            continue;
        } else {
            break;
        }
    }

    // Toggle device state, "1" or "0"
    char device_state[2] = "0";

    while (1) {
        // Toggle the state
        device_state[0] = (device_state[0] == '0') ? '1' : '0';

        // Write state to NVS
        if (write_device_state(my_handle, device_state) == ESP_OK) {
            printf("Device state %s written to NVS\n", device_state);
        }

        // Sleep for 1 second before next write
        sleep(1);
    }

    // Commit written value to NVS storage
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS: %s\n", esp_err_to_name(err));
    }

    // Close NVS handle
    nvs_close(my_handle);
 
    return 0;
}