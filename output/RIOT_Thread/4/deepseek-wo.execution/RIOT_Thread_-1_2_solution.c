
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char thread_stack[STACKSIZE];
static msg_t msg_queue[NUM_MESSAGES];

static void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    for (int i = 0; i < 5; i++) {
        msg_send_receive(&msg, &msg, thread_getpid());
        printf("Sender: Sent message with value %lu\n", msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg, reply;
    reply.type = 2;
    reply.content.value = 100;

    for (int i = 0; i < 5; i++) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(thread_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(thread_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                            THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
