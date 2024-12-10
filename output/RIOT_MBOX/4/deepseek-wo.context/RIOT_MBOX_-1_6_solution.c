
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "mbox.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_MS         (1000)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, -1);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_MSEC);

    while (1) {
        msg_t msg;
        msg.content.value = 1;
        mbox_put(&mbox, &msg);
        ztimer_set(&timer, INTERVAL_MS);
        ztimer_wait(&timer);
    }

    return 0;
}
