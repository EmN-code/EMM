
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stddef.h>
#include <stdatomic.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void toggle_state(void)
{
    nvs_handle_t handle;
    uint8_t state = 0;
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);

    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    state = (state == 0) ? 1 : 0;
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
    nvs_close(handle);
}

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, &mbox);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_msg(&evtimer);

    ztimer_set(ZTIMER_MSEC, &evtimer.timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        toggle_state();
        ztimer_set(ZTIMER_MSEC, &evtimer.timer, 1000);
    }

    return 0;
}
