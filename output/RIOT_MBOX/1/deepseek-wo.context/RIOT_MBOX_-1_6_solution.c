
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "ztimer.h"
#include "mbox.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

static void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_t *event_thread_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        event_thread,
        NULL,
        "event_thread");

    if (event_thread_pid == NULL) {
        printf("Failed to create event thread\n");
        return 1;
    }

    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        ztimer_wait(&timer);
        msg_t msg;
        msg.content.value = 42; // Example event value
        mbox_put(&mbox, &msg);
    }

    return 0;
}
