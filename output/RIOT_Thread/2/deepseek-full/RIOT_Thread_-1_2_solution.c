
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        msg_t msg;
        msg.content.value = i;
        msg.type = 1;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Sleep for 1 second between messages
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    receiver_pid = thread_create(receiver_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    sender_pid = thread_create(sender_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, sender_thread, NULL, "sender");

    return 0;
}
