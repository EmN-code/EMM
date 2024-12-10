
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.value = 42; // Example message content

    printf("High priority thread sending message to low priority thread\n");
    msg_send(&msg, pid_low);

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;

    msg_t msg;

    printf("Low priority thread waiting for message\n");
    msg_receive(&msg);

    printf("Low priority thread received message with value: %d\n", msg.content.value);

    return NULL;
}

int main(void)
{
    puts("Starting inter-thread communication example");

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    // Main thread can continue its work or simply wait
    while (1) {
        thread_yield();
    }

    return 0;
}
