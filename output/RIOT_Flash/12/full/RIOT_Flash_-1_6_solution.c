#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define STATE_KEY "device_state"

int main(void) {
    // Initialize the default NVS partition
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return err;
    }

    int32_t state = 0;

    while (1) {
        // Set device state persistently
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Error setting value!");
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing!");
        }

        // Flip the state
        state = !state;
        printf("Stored state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle (not reached in this example, due to the infinite loop)
    nvs_close(my_handle);

    return 0;
}