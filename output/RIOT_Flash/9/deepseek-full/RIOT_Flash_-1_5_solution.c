
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "nvs_flash.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "nvs.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "string.h"
#include "unistd.h"
#include "stdatomic.h"

#define QUEUE_SIZE 8
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static nvs_handle_t nvs_handle;

void toggle_state(void) {
    uint8_t state;
    nvs_get_u8(nvs_handle, NVS_KEY, &state);
    state = (state == 0) ? 1 : 0;
    nvs_set_u8(nvs_handle, NVS_KEY, state);
    nvs_commit(nvs_handle);
    printf("Device state toggled to: %d\n", state);
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, mbox_pid(&mbox));
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    assert(err == ESP_OK);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    assert(err == ESP_OK);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        toggle_state();
    }

    nvs_close(nvs_handle);
    return 0;
}
