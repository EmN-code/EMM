
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static mbox_t mbox;
static msg_t msg_array[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer with the message box handler
    evtimer_init_mbox(&evtimer);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, &mbox, (msg_t *)periodic_callback, NULL, PERIODIC_INTERVAL);

    // Start the periodic event
    evtimer_start(&evtimer);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
