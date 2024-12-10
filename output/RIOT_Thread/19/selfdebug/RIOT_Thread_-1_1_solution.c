#include "thread.h"
#include "msg.h"
#include "sched.h"
#include "stdio.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];
static msg_t msg_queue[8];

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Low priority thread received a message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary number
    printf("High priority thread sending message.\n");
    if (msg_send(&msg, thread_getpid_of((thread_t *)arg)) == 1) {
        printf("High priority thread successfully sent a message.\n");
    }
    else {
        printf("Message send failed.\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_pid = thread_create(stack1, STACK_SIZE, PRIORITY_LOW, 0, thread_low, NULL, "low");
    if (low_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create low priority thread\n");
        return -1;
    }

    kernel_pid_t high_pid = thread_create(stack2, STACK_SIZE, PRIORITY_HIGH, 0, thread_high, &low_pid, "high");
    if (high_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create high priority thread\n");
        return -1;
    }

    msg_init_queue(msg_queue, 8);
    printf("Threads "low" and "high" created. Folder receiving messages.\n");

    return 0;
}