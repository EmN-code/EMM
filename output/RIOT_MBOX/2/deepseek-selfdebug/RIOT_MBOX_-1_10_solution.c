
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "event.h"
#include "mbox.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL_MS         (1000)

static mbox_t mbox;
static msg_t mbox_queue[8];

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    mbox_put(&mbox, &msg);
}

static void mbox_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    while (mbox_get(&mbox, &msg, 0) == 1) {
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, sizeof(mbox_queue) / sizeof(msg_t));

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &mbox, event_callback, NULL);
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    thread_t *mbox_thread = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        mbox_handler,
        NULL,
        "mbox_handler"
    );

    (void)mbox_thread;

    return 0;
}
