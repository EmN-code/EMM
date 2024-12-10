
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "mbox.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

#define MSG_QUEUE_SIZE (8)

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void *event_thread(void *arg)
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
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    xtimer_ticks32_t last_wakeup = xtimer_now();

    for (int i = 0; i < 10; i++) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000); // 1 second interval
        msg_t msg;
        msg.content.value = i;
        mbox_put(&mbox, &msg);
    }

    return 0;
}
