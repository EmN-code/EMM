
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    char state_str[2];
    sprintf(state_str, "%d", state);
    nvs_set_str(handle, NVS_KEY, state_str);
    nvs_commit(handle);
}

int main(void) {
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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        int state = 0;
        while (1) {
            // Toggle state
            state = (state == 0) ? 1 : 0;

            // Store state in NVS
            store_state(my_handle, state);

            // Print current state
            printf("Device state stored: %d\n", state);

            // Wait for 1 second
            sleep(1);
        }

        // Close NVS handle
        nvs_close(my_handle);
    }

    return 0;
}
