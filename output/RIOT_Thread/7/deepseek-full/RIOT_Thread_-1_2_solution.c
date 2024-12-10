
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_sender[STACKSIZE];
static char stack_receiver[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack_sender, sizeof(stack_sender),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                              THREAD_PRIORITY_MAIN - 2, 0,
                                              receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
