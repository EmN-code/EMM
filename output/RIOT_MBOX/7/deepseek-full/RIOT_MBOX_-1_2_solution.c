
#include <stdio.h>
#include <stdlib.h>
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY 5
#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static char stack[STACKSIZE];
static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS * 1000);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(stack, STACKSIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(stack, STACKSIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
