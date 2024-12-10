
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        xtimer_sleep(INTERVAL_MS / MS_IN_USEC);
        msg.content.value = 1; // Dummy event
        evtimer_add_msg(&evtimer, &msg, &mbox);
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
