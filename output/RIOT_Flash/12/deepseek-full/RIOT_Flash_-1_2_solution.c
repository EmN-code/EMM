
#include <nvs_flash.h>
#include <string.h>
#include <nvs.h>

#define DEVICE_STATE_KEY "device_state"

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        // Handle error
        return err;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        // Handle error
        return err;
    }

    while (1) {
        // Toggle state
        state = !state;

        // Store state in NVS
        err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
        if (err != ESP_OK) {
            // Handle error
            nvs_close(my_handle);
            return err;
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            // Handle error
            nvs_close(my_handle);
            return err;
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);
    return ESP_OK;
}
