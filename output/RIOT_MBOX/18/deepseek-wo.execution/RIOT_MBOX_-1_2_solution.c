
#include <stdio.h>
#include <thread.h>
#include <ztimer.h>
#include <evtimer_mbox.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static evtimer_mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg.content.value++;
        evtimer_mbox_add_msg(&mbox, &msg);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        evtimer_mbox_get_msg(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void)
{
    evtimer_mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
