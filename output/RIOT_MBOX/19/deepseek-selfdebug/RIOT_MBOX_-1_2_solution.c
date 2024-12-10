
#include "thread.h"
#include "msg.h"
#include "evtimer.h"
#include "xtimer.h"

#define MSGBOX_SIZE 8
#define EVENT_INTERVAL_MS 1000

static msg_t msg_queue[MSGBOX_SIZE];
static mbox_t msg_box;

static void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example event value
    mbox_put(&msg_box, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&msg_box, msg_queue, MSGBOX_SIZE);

    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer);

    // Add periodic event to the event timer
    evtimer_add_msg(&evtimer, event_handler, NULL, EVENT_INTERVAL_MS * US_PER_MS);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&msg_box, &msg);
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
