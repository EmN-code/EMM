#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char thread_stack[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_pid;
kernel_pid_t thread_pid2;

void *thread_handler(void *arg) {
    msg_t msg;
    (void) arg;
    while (1) {
        if (msg_try_receive(&msg) == 1) {
            printf("Message received: %lu\n", (unsigned long)msg.content.value);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_handler2(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        if (msg_send(&msg, thread_pid) == 1) {
            printf("Message sent: %lu\n", (unsigned long)msg.content.value);
        }
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "thread1");

    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                                THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST,
                                thread_handler2, NULL, "thread2");

    return 0;
}