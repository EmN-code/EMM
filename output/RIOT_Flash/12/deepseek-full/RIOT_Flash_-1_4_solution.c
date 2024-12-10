
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <nvs_flash.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void toggle_state(void) {
    uint8_t state;
    esp_err_t err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        state = 0;
    }
    state = (state == 0) ? 1 : 0;
    err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set state in NVS\n");
        return;
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit state to NVS\n");
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Failed to erase NVS partition\n");
            return -1;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            printf("Failed to initialize NVS partition\n");
            return -1;
        }
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle\n");
        return -1;
    }

    while (1) {
        toggle_state();
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
