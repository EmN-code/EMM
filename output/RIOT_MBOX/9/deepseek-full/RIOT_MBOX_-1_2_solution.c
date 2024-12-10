
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define QUEUE_SIZE (16)
#define INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 1;  // Example message content
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL);
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
