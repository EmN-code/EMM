
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <net/gcoap.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define QUEUE_SIZE (16)
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void store_state(bool state) {
    // Simulate storing state in NVS
    printf("Storing state: %d\n", state);
}

void toggle_state(void) {
    static bool state = false;
    state = !state;
    store_state(state);
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, mbox_pid(&mbox), false);
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Simulate NVS initialization
    printf("NVS initialized\n");

    // Set up a periodic timer to toggle the state every second
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        toggle_state();
    }

    return 0;
}
