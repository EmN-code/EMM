#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char low_priority_stack[THREAD_STACKSIZE];
static char high_priority_stack[THREAD_STACKSIZE];

static msg_t msg_queue[8];

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("[Low Priority] Error receiving message\n");
        } else {
            printf("[Low Priority] Received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        printf("[High Priority] Sending message: %d\n", msg.content.value);
        if (msg_send(&msg, thread_get_lowest()) < 0) {
            printf("[High Priority] Error sending message\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_pid = thread_create(low_priority_stack, sizeof(low_priority_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");
    if (low_pid < 0) {
        printf("Error creating low priority thread\n");
        return 1;
    }

    kernel_pid_t high_pid = thread_create(high_priority_stack, sizeof(high_priority_stack), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");
    if (high_pid < 0) {
        printf("Error creating high priority thread\n");
        return 1;
    }

    printf("Threads created successfully\n");

    return 0;
}