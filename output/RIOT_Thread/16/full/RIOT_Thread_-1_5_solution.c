#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE     (THREAD_STACKSIZE_DEFAULT)

char stack1[STACKSIZE];
char stack2[STACKSIZE];
msg_t msg_q[8];

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message in high priority thread: %d\n", msg.content.value);
    }
    return NULL;
}

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_getpid());
        printf("Message sent from low priority thread\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t high_pid = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0,
                                          thread_high, NULL, "high_thread");
    if (high_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create high priority thread\n");
        return 1;
    }

    kernel_pid_t low_pid = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0,
                                         thread_low, NULL, "low_thread");
    if (low_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create low priority thread\n");
        return 1;
    }

    msg_init_queue(msg_q, sizeof(msg_q) / sizeof(msg_q[0]));

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}