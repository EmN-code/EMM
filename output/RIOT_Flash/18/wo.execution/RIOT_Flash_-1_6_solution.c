#include <xtimer.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing state to NVS!\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing state to NVS!\n", err);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    uint8_t device_state = 1; // Example initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    }

    while (true) {
        // Update device state every second
        store_device_state(my_handle, device_state);
        xtimer_sleep(1);
        // Toggle state for demonstration
        device_state = !device_state;
    }

    // Close NVS handle (unreachable code, for demonstration)
    nvs_close(my_handle);

    return 0;
}