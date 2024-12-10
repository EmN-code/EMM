
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "mbox.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example event value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&mbox);

    // Set up the event to trigger every 2 seconds
    evtimer_add_msg(&mbox, ZTIMER_SEC, 2, event_callback, NULL);

    msg_t msg;
    while (1) {
        // Wait for a message from the mailbox
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received event with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
