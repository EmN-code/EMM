
#include "mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define QUEUE_SIZE (16)  // Must be a power of two

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Example message content

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Send a message every second
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
