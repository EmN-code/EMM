
#include "kernel_defines.h"
#include "string.h"
#include "stddef.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "nvs_flash.h"
#include "stdlib.h"
#include "nvs.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "stdio.h"

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
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Simulate temperature update
        uint8_t current_state = read_device_state(handle);
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_device_state(handle, new_state);

        // Print current state
        printf("Device state updated to: %d\n", new_state);

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
