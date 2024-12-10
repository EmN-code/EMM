
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static bool device_state = false;

void update_state_and_store() {
    device_state = !device_state; // Toggle state
    int8_t state_to_store = device_state ? 1 : 0;

    esp_err_t err = nvs_set_i8(my_handle, DEVICE_STATE_KEY, state_to_store);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %d\n", err);
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS: %d\n", err);
    }
}

int main() {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %d\n", err);
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        update_state_and_store();
        xtimer_sleep(1); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
