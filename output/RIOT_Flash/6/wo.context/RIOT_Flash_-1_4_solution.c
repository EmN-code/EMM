// Including necessary headers for RIOT and ESP32
#include "xtimer.h"  // for using timers
#include "nvs_flash.h"  // for NVS functions
#include "nvs.h"  // for NVS handle and operations

/* The device will store its state in NVS every second. If the device state
 * equals '1', it indicates one state and '0' for another.
 */

// Setting up the storage namespace and key
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

// Function to initialize NVS
void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

// Function to update the device state in NVS
void update_device_state(nvs_handle_t my_handle, int state) {
    esp_err_t err = nvs_set_i32(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }
}

// Main function
int main(void) {
    init_nvs();

    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return err;
    }

    int current_state = 0;

    while (1) {
        // Toggle state between 0 and 1
        current_state = !current_state;
        update_device_state(my_handle, current_state);

        printf("Updated device state to: %d
", current_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle (this point is never actually reached in this loop)
    nvs_close(my_handle);
    return 0;
}
