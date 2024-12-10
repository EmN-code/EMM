#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"
#include "esp_err.h"

// Function declarations
void initialize_nvs(void);
void store_device_state(int state);
int read_device_state(void);

int main() {
    // Initialize the NVS
    initialize_nvs();

    // Main loop to update state every second
    while (1) {
        // Simulate device state (flip between 1 and 0 every second for example)
        static int state = 0;
        state = !state;

        // Store state in NVS
        store_device_state(state);

        // Read back state from NVS for verification
        int stored_state = read_device_state();
        printf("Stored state: %d\n", stored_state);

        // Sleep for one second before the next update
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
}

void initialize_nvs(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    // Write
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return;
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return;
    }

    // Close
    nvs_close(my_handle);
}

int read_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0; // Default state if not found

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return -1; // Error indicator

    // Read
    err = nvs_get_i32(my_handle, "device_state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        nvs_close(my_handle);
        return -1;
    }

    // Close
    nvs_close(my_handle);

    return state;
}