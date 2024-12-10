
#include <stdlib.h>
#include <nvs_flash.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <string.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err;
    uint8_t device_state = 0;

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
        // Toggle device state
        device_state = !device_state;

        // Update device state in NVS
        update_device_state(handle, device_state);

        // Print current state
        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
