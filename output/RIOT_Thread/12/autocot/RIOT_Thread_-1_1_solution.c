#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <xtimer.h>
#include <mutex.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE            (THREAD_STACKSIZE_MAIN)

char stack_low[STACKSIZE];
char stack_high[STACKSIZE];

msg_t msg_queue_low[8];
msg_t msg_queue_high[8];

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_low, 8);
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Low priority thread received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_high, 8);
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(2);
        printf("High priority thread sending message\n");

        if (msg_send(&msg, thread_getpid_of((thread_t *)arg)) == 1) {
            printf("Message sent successfully\n");
        } else {
            printf("Message sending failed\n");
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_LOW, 0,
                                         low_priority_thread, NULL, "low");

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_HIGH, 0,
                                          high_priority_thread, (void *)low_pid, "high");

    printf("Threads started: low=%d, high=%d\n", low_pid, high_pid);

    return 0;
}