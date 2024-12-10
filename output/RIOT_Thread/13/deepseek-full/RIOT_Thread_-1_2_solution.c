
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char thread_stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message from sender\n");

        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;

    while (1) {
        msg.content.value = 100;
        msg.sender_pid = thread_getpid();

        msg_send_receive(&msg, &reply, receiver_pid);
        printf("Sender: Received reply with value %" PRIu32 "\n", reply.content.value);

        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    receiver_pid = thread_create(thread_stack, sizeof(thread_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 2, 0,
                               sender_thread, NULL, "sender");

    return 0;
}
