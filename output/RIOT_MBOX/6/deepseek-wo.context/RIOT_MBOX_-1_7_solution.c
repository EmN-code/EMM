
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define SENDER_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
static char _sender_stack[SENDER_STACKSIZE_DEFAULT];

static mbox_t mbox;
static msg_t mbox_msgs[8];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(2);
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_msgs, 8);

    kernel_pid_t sender_pid = thread_create(_sender_stack, sizeof(_sender_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 0);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
