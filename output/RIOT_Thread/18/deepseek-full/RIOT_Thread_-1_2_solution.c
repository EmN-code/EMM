
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_buffer_sender[STACKSIZE];
static char stack_buffer_receiver[STACKSIZE];

static msg_t msg_array[NUM_MESSAGES];
static msg_t msg_queue[NUM_MESSAGES];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
    }
    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_array, NUM_MESSAGES);

    sender_pid = thread_create(stack_buffer_sender, sizeof(stack_buffer_sender),
                               THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender");

    receiver_pid = thread_create(stack_buffer_receiver, sizeof(stack_buffer_receiver),
                                 THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    return 0;
}
