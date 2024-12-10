
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack[STACKSIZE];
static msg_t msg_queue[NUM_MESSAGES];

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message from PID %d with type %u\n", msg.sender_pid, msg.type);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.sender_pid = thread_getpid();
        msg_send(&msg, (kernel_pid_t)arg);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t receiver_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                              THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    kernel_pid_t sender_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                            THREAD_CREATE_STACKTEST, sender_thread, (void *)receiver_pid, "sender");

    (void)sender_pid; // To suppress unused variable warning

    return 0;
}
