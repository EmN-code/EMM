
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    receiver_pid = thread_create(stack, sizeof(stack), PRIORITY,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    msg_t msg;
    msg.type = 0;
    msg.content.ptr = "Hello, RIOT!";

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
    }

    return 0;
}
