
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, thread_getpid());
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
