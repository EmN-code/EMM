#include "xtimer.h"
#include "kernel_defines.h"
#include "evtimer.h"
#include "periph/gpio.h"
#include "msg.h"

#define INTERVAL (1U * US_PER_SEC)
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 1;  // example payload

    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);
        puts("Sending message...");
        msg_send(&msg, thread_getpid_of(receiver_thread));
    }
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
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    printf("Sender thread PID: %d\n", sender_pid);
    printf("Receiver thread PID: %d\n", receiver_pid);

    // Start message handling in both threads
    while (1) {
        evtimer_add_mbox(&evtimer, &(mbox_event), &msg_queue);
    }

    return 0;
}