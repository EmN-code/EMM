
#include <string.h>
#include <stddef.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <stdatomic.h>
#include <nvs_flash.h>
#include <stdlib.h>
#include <nvs.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <stdio.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing device state: %d\n", err);
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;
    uint8_t device_state = 0;

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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return err;
    }

    while (1) {
        // Toggle device state
        device_state = !device_state;

        // Update device state in NVS
        update_device_state(handle, device_state);

        // Print current device state
        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
