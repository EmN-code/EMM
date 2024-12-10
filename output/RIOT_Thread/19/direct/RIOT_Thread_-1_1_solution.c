#include <string.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE (8)

char thread_stack_low[STACKSIZE];
char thread_stack_high[STACKSIZE];

msg_t msg_queue_low[MSG_QUEUE_SIZE];
msg_t msg_queue_high[MSG_QUEUE_SIZE];

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_low = thread_create(thread_stack_low, sizeof(thread_stack_low), THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");
    kernel_pid_t pid_high = thread_create(thread_stack_high, sizeof(thread_stack_high), THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    if (pid_low <= KERNEL_PID_UNDEF || pid_high <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    printf("Threads started\n");

    for (int i = 0; i < 10; ++i) {
        msg_t msg;
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_low);
        } else {
            msg_send(&msg, pid_high);
        }
        xtimer_sleep(1);
    }
    return 0;
}
