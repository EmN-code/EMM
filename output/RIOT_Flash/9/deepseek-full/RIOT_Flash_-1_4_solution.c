
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static msg_t msg_queue[8];
static mbox_t mbox;

void toggle_state(void) {
    static bool state = false;
    state = !state;

    // Simulate storing the state persistently
    printf("Device state toggled to: %d\n", state);

    // Schedule the next toggle
    ztimer_set(ZTIMER_MSEC, &mbox.timer, 1000);
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Schedule the first toggle
    ztimer_set(ZTIMER_MSEC, &mbox.timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        // Handle the message (in this case, just toggle the state)
        toggle_state();
    }

    return 0;
}
