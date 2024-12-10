
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t msg_array[10];

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, -1);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, msg_array, 10);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    xtimer_ticks32_t last_wakeup = xtimer_now();

    for (int i = 0; i < 10; i++) {
        msg_t msg;
        msg.content.value = i;
        mbox_put(&mbox, &msg);
        xtimer_periodic_wakeup(&last_wakeup, 1000000); // 1 second interval
    }

    return 0;
}
