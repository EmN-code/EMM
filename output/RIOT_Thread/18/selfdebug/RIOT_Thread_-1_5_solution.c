#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "thread.h"

#define THREAD_STACKSIZE_MAIN 2048
#define THREAD_PRIORITY_MAIN 5

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Thread received message with value: %u\n", (unsigned)msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("RIOT OS: Thread Communication Example\n");

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    kernel_pid_t thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "thread");

    msg_t msg;
    for (int i = 0; i < 10; i++) {
        msg.content.value = (void *)(uintptr_t)i; // Correctly cast integer to void pointer
        if (msg_send(&msg, thread_pid) < 1) {
            printf("Error sending message\n");
        }
    }

    return 0;
}