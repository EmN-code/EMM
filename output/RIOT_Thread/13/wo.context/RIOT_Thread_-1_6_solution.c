#include <stdio.h>
#include <string.h>
#include <ztimer.h>
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE   (8)

char thread_one_stack[THREAD_STACKSIZE];
char thread_two_stack[THREAD_STACKSIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = (uint32_t)100;  // Ensuring the value is correct type

    while(1) {
        msg_send(&msg, thread_two_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;

    while(1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", (unsigned long)msg.content.value);  // Using correct format specifier
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY_MAIN - 2, 0, thread_two, NULL, "thread_two");

    return 0;
}