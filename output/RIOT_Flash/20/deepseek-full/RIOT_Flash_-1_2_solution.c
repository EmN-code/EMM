
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, NVS_KEY, state);
    nvs_commit(handle);
}

uint8_t toggle_state(uint8_t current_state) {
    return current_state == 0 ? 1 : 0;
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t current_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Store the current state
        store_state(my_handle, current_state);

        // Toggle the state
        current_state = toggle_state(current_state);

        // Delay for 1 second
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
