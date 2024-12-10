#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        sleep(1); // Wait a second before retry
        err = nvs_flash_init();
    }
}

void store_device_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        sleep(1); // Wait before retrying
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }

    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return;
    }

    err = nvs_commit(my_handle);
    if (err == ESP_OK) {
        printf("Device state committed successfully: %d\n", state);
    } else {
        printf("Error committing device state: %s\n", esp_err_to_name(err));
    }

    nvs_close(my_handle);
}

int main(void) {
    initialize_nvs();
    uint8_t state = 0;

    while(1) {
        state = !state; // toggle state between 0 and 1
        store_device_state(state);
        sleep(1); // Sleep for 1 second before updating state again
    }

    return 0;
}
