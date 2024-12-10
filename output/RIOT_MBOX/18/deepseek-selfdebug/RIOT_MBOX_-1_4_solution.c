
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char _thread_stack[THREAD_STACKSIZE];
static mbox_t _mbox;

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&_mbox, &msg, -1);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&_mbox, NULL);

    thread_create(_thread_stack, THREAD_STACKSIZE, THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&_mbox, &msg);
        xtimer_sleep(1);
    }

    return 0;
}
