
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define HIGH_PRIO (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIO (THREAD_PRIORITY_MAIN - 2)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    printf("High priority thread started\n");

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.ptr = "Hello from low priority thread";

    printf("Low priority thread started\n");

    while (1) {
        msg_send(&msg, pid_high);
        printf("Low priority thread sent message\n");
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT inter-thread communication example");

    pid_high = thread_create(stack_high, sizeof(stack_high), HIGH_PRIO,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), LOW_PRIO,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    return 0;
}
