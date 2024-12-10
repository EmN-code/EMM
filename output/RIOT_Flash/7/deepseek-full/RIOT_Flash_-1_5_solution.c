
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t my_handle;
static bool device_state = false;

void toggle_state(void) {
    device_state = !device_state;
}

void store_state(void) {
    int err;
    uint8_t state_to_store = device_state ? 1 : 0;
    err = nvs_set_u8(my_handle, NVS_KEY, state_to_store);
    if (err != ESP_OK) {
        printf("Error storing state: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing state: %s\n", esp_err_to_name(err));
    }
}

void read_state(void) {
    int err;
    uint8_t state;
    err = nvs_get_u8(my_handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        printf("Error reading state: %s\n", esp_err_to_name(err));
        return;
    }
    device_state = state == 1;
}

void periodic_task(void) {
    toggle_state();
    store_state();
    printf("Device state updated to: %d\n", device_state);
}

int main(void) {
    int err;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    read_state();
    printf("Initial device state: %d\n", device_state);

    while (1) {
        periodic_task();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(my_handle);
    return 0;
}
