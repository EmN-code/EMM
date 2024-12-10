#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define HIGH_PRIORITY_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY_THREAD_PRIO  (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL (1U * US_PER_SEC) // 1 second

char high_priority_stack[THREAD_STACKSIZE_MAIN];
char low_priority_stack[THREAD_STACKSIZE_MAIN];

void *high_priority_thread(void *arg) {
    msg_t msg;
    (void) arg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("High priority thread received a message: %ld\n", msg.content.value);
        }
        xtimer_sleep(2); // Delay to simulate work
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg.content.value = xtimer_usec_from_ticks(xtimer_now()); // Simulate sending a message with timestamp
        msg_send(&msg, thread_get(HIGH_PRIORITY_THREAD_PRIO));
        printf("Low priority thread sent a message\n");

        xtimer_sleep(1); // Interval between messages
    }
    return NULL;
}

int main(void) {
    thread_create(high_priority_stack, sizeof(high_priority_stack),
                  HIGH_PRIORITY_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  high_priority_thread, NULL, "high_priority_thread");
    
    thread_create(low_priority_stack, sizeof(low_priority_stack),
                  LOW_PRIORITY_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  low_priority_thread, NULL, "low_priority_thread");

    return 0;
}