
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>

#define QUEUE_SIZE 8
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void update_state(void) {
    static bool state = false;
    state = !state;
    printf("Device state updated to: %d\n", state);
    // Simulate storing the state persistently
    // This is a placeholder as NVS functions are not allowed
    // In a real scenario, you would use NVS functions here
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, (kernel_pid_t)mbox.msg_array);
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_mbox_t et;
    evtimer_mbox_init(&et, &mbox);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        update_state();
    }

    return 0;
}
