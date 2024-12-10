
#include "mbox.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "stdatomic.h"
#include "nvs.h"
#include "time_units.h"
#include "stdlib.h"
#include "string.h"
#include "evtimer_mbox.h"
#include "stdbool.h"
#include "ztimer.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

uint8_t read_state(void) {
    uint8_t state = 0;
    nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Simulate temperature update
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("Updated state: %d\n", new_state);

        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
