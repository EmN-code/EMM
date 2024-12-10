
#include <nvs.h>
#include <stdlib.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <esp_wifi.h>
#include <time_units.h>
#include <esp_system.h>
#include <string.h>

#define NVS_NAMESPACE "storage"
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
        return err;
    }

    while (1) {
        // Simulate device state (1 or 0)
        int device_state = rand() % 2;

        // Store the device state in NVS
        store_device_state(my_handle, device_state);

        // Print the stored state
        printf("Device state stored: %d\n", device_state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
