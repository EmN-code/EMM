#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        nvs_flash_erase();
        nvs_flash_init();
    }

    // Open
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    } else {
        int32_t device_state = 0;
        while (1) {
            // Simulating a device state change
            device_state = !device_state; // Toggle between 0 and 1

            // Write
            err = nvs_set_i32(my_handle, "device_state", device_state);
            if (err != ESP_OK) {
                printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            }

            // Read back value
            int32_t value = 0; // read value placeholder
            err = nvs_get_i32(my_handle, "device_state", &value);
            if (err == ESP_OK) {
                printf("Device state = %ld\n", (long)value); // Fixed format specifier issue
            } else {
                printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
            }

            // Simulate waiting for 1 second
            time_t endwait;
            time_t start = time(NULL);
            endwait = start + 1;
            while (start < endwait)
                start = time(NULL);
        }

        // Close
        nvs_close(my_handle);
    }

    return ESP_OK;
}