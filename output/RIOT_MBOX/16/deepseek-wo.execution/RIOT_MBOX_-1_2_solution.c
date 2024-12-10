
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        _mbox_put(&mbox, &msg, 0);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        _mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
