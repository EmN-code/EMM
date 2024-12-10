
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void store_state(uint8_t state) {
    nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    nvs_commit(my_handle);
}

uint8_t read_state(void) {
    uint8_t state = 0;
    nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    int err;
    uint8_t current_state = 0;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Toggle state
        current_state = (current_state == 0) ? 1 : 0;
        store_state(current_state);

        xtimer_usleep(1000000); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
