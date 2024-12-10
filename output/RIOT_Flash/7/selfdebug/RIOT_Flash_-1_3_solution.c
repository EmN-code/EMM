#include <xtimer.h>  // For delays and timers
#include <stdio.h>    // Standard input/output
#include <nvs_flash.h> // For NVS functions

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY         "device_state"

static nvs_handle_t nvs_handle;

void init_nvs(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %s\n", esp_err_to_name(err));
        return;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
}

void store_device_state(const char* state) {
    // Write the value to NVS
    esp_err_t err = nvs_set_str(nvs_handle, STATE_KEY, state);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }
    if (err != ESP_OK) {
        printf("Failed to write to NVS: %s\n", esp_err_to_name(err));
    }
    else {
        printf("Stored state: %s\n", state);
    }
}

int main(void) {
    init_nvs();

    char current_state[2] = "0";

    while (1) {
        store_device_state(current_state);

        // Toggle state for demonstration
        current_state[0] = (current_state[0] == '0') ? '1' : '0';

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Never reached
    nvs_close(nvs_handle);
    return 0;
}