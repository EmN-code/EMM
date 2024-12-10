
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <timex.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static atomic_bool device_state = ATOMIC_VAR_INIT(0);

void toggle_state(void) {
    atomic_store(&device_state, !atomic_load(&device_state));
}

void store_state(void) {
    uint8_t state = atomic_load(&device_state);
    nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    nvs_commit(my_handle);
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        toggle_state();
        store_state();
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
