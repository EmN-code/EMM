
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <event/thread.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

char stack_buffer_high[STACKSIZE];
char stack_buffer_low[STACKSIZE];

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

msg_t msg_queue_high[MSG_QUEUE_SIZE];
msg_t msg_queue_low[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    high_priority_pid = thread_create(stack_buffer_high, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, high_priority_thread, NULL, "high_priority");
    low_priority_pid = thread_create(stack_buffer_low, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    // Send a message to the high priority thread
    msg_send(&msg, high_priority_pid);

    // Send a message to the low priority thread
    msg.content.value = 24;
    msg_send(&msg, low_priority_pid);

    return 0;
}
