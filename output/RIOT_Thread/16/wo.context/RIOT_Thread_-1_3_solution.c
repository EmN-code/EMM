#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <jemalloc/jemalloc.h>
#include <event.h>
#include <event_thread.h>
#include <kernel_types.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_LARGE (THREAD_STACKSIZE_DEFAULT + 512)

char thread_stack_1[THREAD_STACKSIZE_LARGE];
char thread_stack_2[THREAD_STACKSIZE_LARGE];

kernel_pid_t thread_pid_1;
kernel_pid_t thread_pid_2;

msg_t msg_queue[8];

void *thread_handler_1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_pid_2);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_handler_2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                 THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST,
                                 thread_handler_1, NULL, "thread_1");

    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                 THREAD_PRIORITY_MAIN - 2,
                                 THREAD_CREATE_STACKTEST,
                                 thread_handler_2, NULL, "thread_2");
    
    return 0;
}