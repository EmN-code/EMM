
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <stddef.h>
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

static void update_state(nvs_handle_t handle) {
    static atomic_bool state = false;
    state = !state;
    uint8_t state_value = state ? 1 : 0;
    nvs_set_u8(handle, DEVICE_STATE_KEY, state_value);
    nvs_commit(handle);
    printf("Device state updated to: %d\n", state_value);
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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Update state every second
    while (1) {
        update_state(handle);
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached due to the infinite loop)
    nvs_close(handle);

    return 0;
}
