
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static atomic_bool state = ATOMIC_VAR_INIT(false);

void toggle_state(void) {
    bool current_state = atomic_load(&state);
    bool new_state = !current_state;
    atomic_store(&state, new_state);

    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, new_state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Failed to set NVS value: %d\n", err);
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit NVS value: %d\n", err);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle: %s\n", esp_err_to_name(err));
        return 1;
    }

    uint8_t stored_state;
    err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &stored_state);
    if (err == ESP_OK) {
        atomic_store(&state, stored_state == 1);
    } else {
        printf("Failed to get NVS value: %s\n", esp_err_to_name(err));
    }

    while (1) {
        toggle_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(my_handle);
    return 0;
}
