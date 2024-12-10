
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define HIGH_PRIO (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIO (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

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
    char *message = "Hello from low priority thread!";

    while (1) {
        xtimer_sleep(5);  // Sleep for 5 seconds
        msg.content.ptr = message;
        msg_send(&msg, pid_high);
        printf("Low priority thread sent message to high priority thread\n");
    }

    return NULL;
}

int main(void)
{
    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), HIGH_PRIO,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_prio");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), LOW_PRIO,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_prio");

    // Main thread can also send messages
    msg_t msg;
    char *main_message = "Hello from main thread!";

    while (1) {
        xtimer_sleep(10);  // Sleep for 10 seconds
        msg.content.ptr = main_message;
        msg_send(&msg, pid_high);
        printf("Main thread sent message to high priority thread\n");
    }

    return 0;
}
