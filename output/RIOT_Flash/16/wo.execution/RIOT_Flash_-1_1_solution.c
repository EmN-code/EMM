#include <xtimer.h>  // Include timer related functions
#include <stdio.h>
#include <nvs_flash.h>  // Include NVS (Non Volatile Storage) flash management

#define NVS_NAMESPACE "storage"  // Define storage namespace
#define STATE_KEY "device_state"  // Define the key to store device state

void initialize_nvs() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void save_state_nvs(int state) {
    nvs_handle_t storage_handle;
    // Open NVS namespace
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &storage_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS!");
        return;
    }
    // Write state to NVS
    err = nvs_set_i32(storage_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error writing to NVS!");
    }
    // Commit written value
    err = nvs_commit(storage_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS!");
    }
    // Close NVS handle
    nvs_close(storage_handle);
}

void read_state_nvs(int *state) {
    nvs_handle_t storage_handle;
    // Open NVS namespace
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &storage_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS!");
        return;
    }
    // Read state from NVS
    err = nvs_get_i32(storage_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The state is not initialized yet!");
        } else {
            printf("Error reading from NVS!");
        }
    }
    // Close NVS handle
    nvs_close(storage_handle);
}

int main(void) {
    int state = 0;
    initialize_nvs(); // Initialize the NVS

    while(1) {
        // Toggle state between 0 and 1
        state = (state == 0) ? 1 : 0;

        // Save the current state to NVS every second
        save_state_nvs(state);
        printf("Saved state to NVS: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }
    return 0;
}