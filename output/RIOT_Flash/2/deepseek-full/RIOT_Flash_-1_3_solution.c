
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

uint8_t read_state(nvs_handle_t handle) {
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

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Simulate temperature update
        uint8_t current_state = read_state(handle);
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(handle, new_state);

        printf("Device state updated to: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
