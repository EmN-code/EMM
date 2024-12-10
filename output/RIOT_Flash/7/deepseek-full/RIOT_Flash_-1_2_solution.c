
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DEVICE_STATE_KEY "device_state"
#define DEVICE_STATE_ON "1"
#define DEVICE_STATE_OFF "0"

void store_device_state(nvs_handle_t handle, const char* state) {
    nvs_set_str(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

const char* read_device_state(nvs_handle_t handle) {
    size_t required_size;
    nvs_get_str(handle, DEVICE_STATE_KEY, NULL, &required_size);
    char* state = (char*)malloc(required_size);
    nvs_get_str(handle, DEVICE_STATE_KEY, state, &required_size);
    return state;
}

int main() {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Simulate temperature update and store state
        const char* current_state = read_device_state(my_handle);
        if (strcmp(current_state, DEVICE_STATE_ON) == 0) {
            store_device_state(my_handle, DEVICE_STATE_OFF);
        } else {
            store_device_state(my_handle, DEVICE_STATE_ON);
        }
        free((void*)current_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return 0;
}
