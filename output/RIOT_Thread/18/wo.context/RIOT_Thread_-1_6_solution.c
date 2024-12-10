#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN + 1)

static char stack_high[THREAD_STACKSIZE_MAIN];
static char stack_low[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread_pid_high;
static kernel_pid_t thread_pid_low;

// Message queue and message buffer
static msg_t msg_queue_high[8];
static msg_t msg_queue_low[8];

void *thread_high(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg); // Receive message
        printf("High priority thread received message\n");
        // Process message
    }
    return NULL;
}

void *thread_low(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg); // Receive message
        printf("Low priority thread received message\n");
        // Process message
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue_high, sizeof(msg_queue_high) / sizeof(msg_t));
    msg_init_queue(msg_queue_low, sizeof(msg_queue_low) / sizeof(msg_t));

    thread_pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, 0,
                                    thread_high, NULL, "high priority");

    thread_pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0,
                                   thread_low, NULL, "low priority");

    while(1) {
        msg_t msg;
        msg.content.value = 42; // Example message content

        // Send message to high priority
        msg_send(&msg, thread_pid_high);
        printf("Message sent to high priority thread\n");

        // Send message to low priority
        msg_send(&msg, thread_pid_low);
        printf("Message sent to low priority thread\n");

        xtimer_sleep(2);
    }

    return 0;
}