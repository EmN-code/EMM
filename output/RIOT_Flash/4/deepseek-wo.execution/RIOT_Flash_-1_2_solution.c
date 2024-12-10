
#include <esp_wifi.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>
#include <nvs.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <esp_system.h>
#include <time_units.h>
#include <unistd.h>
#include <nvs_flash.h>
#include <stdio.h>

#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

int main() {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return err;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return err;
    }

    // Simulate device state (1 or 0)
    int device_state = 1;

    while (1) {
        // Store the device state in NVS
        store_device_state(my_handle, device_state);

        // Toggle the device state
        device_state = (device_state == 1) ? 0 : 1;

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
