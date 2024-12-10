
#include "evtimer_msg.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "ztimer.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "xtimer.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example message content
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    xtimer_t timer;
    timer.callback = event_handler;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // Set timer for 1 second

    msg_t received_msg;
    while (1) {
        mbox_get(&mbox, &received_msg, 0); // Non-blocking get
        if (received_msg.content.value == 1) {
            printf("Message received!\n");
        }
    }

    return 0;
}
