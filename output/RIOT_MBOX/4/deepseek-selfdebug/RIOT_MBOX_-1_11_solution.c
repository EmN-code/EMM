
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    while (1) {
        msg_t msg;
        msg.content.value = 1;
        msg_send(&msg, thread_getpid());
        xtimer_sleep(5);
    }

    return 0;
}
