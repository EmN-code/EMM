
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <nvs_flash.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static atomic_int device_state = 0;

void toggle_state(void) {
    device_state = !device_state;
}

void store_state(void) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, device_state);
    if (err != ESP_OK) {
        printf("Error storing state: %d\n", err);
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing state: %d\n", err);
        }
    }
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error erasing NVS partition: %d\n", err);
            return -1;
        }
        // Retry initialization
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return -1;
    }

    while (1) {
        toggle_state();
        store_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
