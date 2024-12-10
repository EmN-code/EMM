
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t my_handle;
static mbox_t state_mbox;
static msg_t state_msg;

void toggle_state(void) {
    uint8_t state;
    nvs_get_u8(my_handle, NVS_KEY, &state);
    state = (state == 0) ? 1 : 0;
    nvs_set_u8(my_handle, NVS_KEY, state);
    nvs_commit(my_handle);
    printf("State toggled to: %d\n", state);
}

void periodic_task(void) {
    while (1) {
        toggle_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    msg_init_queue(state_msg, 10);
    mbox_init(&state_mbox, state_msg, 10);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    periodic_task();

    return 0;
}
