
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

void store_device_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Simulate temperature update
        uint8_t current_state = read_device_state(handle);
        current_state = (current_state == 1) ? 0 : 1; // Toggle state
        store_device_state(handle, current_state);

        printf("Device state updated to: %d\n", current_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
