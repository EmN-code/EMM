#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "sys/socket.h"

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

void store_state(int state) {
    nvs_handle_t handle;
    esp_err_t err;

    while (1) {
        err = nvs_open("storage", NVS_READWRITE, &handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle! Retrying...\n", err);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        err = nvs_set_i32(handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%s) setting value! Retrying...\n", err);
            nvs_close(handle);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing updates! Retrying...\n", err);
            nvs_close(handle);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        nvs_close(handle);
        break;
    }
}

void toggle_and_store_state() {
    static int current_state = 0;
    while (1) {
        current_state = !current_state;  // Toggle state
        store_state(current_state);
        printf("State stored: %d\n", current_state);
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Wait for 1 second
    }
}

int main(void) {
    init_nvs();
    toggle_and_store_state();
    return 0;
}