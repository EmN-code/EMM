#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <msg.h>
#include <xtimer.h>

#define MAIN_QUEUE_SIZE (8)
#define INTERVAL 1000000

static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *thread_func(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", (uint32_t)msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid = thread_create(thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread");

    while (1) {
        msg_t msg;
        msg.type = (uint32_t)rand();
        msg_send(&msg, thread_pid);
        printf("Sent message with type: %u\n", (uint32_t)msg.type);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}