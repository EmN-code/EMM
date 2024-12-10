#include <thread.h>
#include <msg.h>
#include <event.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN)

static char thread_stack_high[STACKSIZE];
static char thread_stack_low[STACKSIZE];

static kernel_pid_t pid_high;
static kernel_pid_t pid_low;

void *thread_handler_high(void *arg) {
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            // Handle error
            continue;
        }
        // Process the message
        printf("High priority thread: Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *thread_handler_low(void *arg) {
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            // Handle error
            continue;
        }
        // Process the message
        printf("Low priority thread: Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_t msg;

    /* Start Threads with Different Priorities */
    pid_high = thread_create(thread_stack_high, sizeof(thread_stack_high), PRIORITY_HIGH, 0,
                            thread_handler_high, NULL, "high");

    pid_low = thread_create(thread_stack_low, sizeof(thread_stack_low), PRIORITY_LOW, 0,
                           thread_handler_low, NULL, "low");

    /* Send Messages to Threads */
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        msg_send(&msg, pid_high);  // Send message to high priority thread
        printf("Main thread: Sent message %d to high priority thread\n", i);

        msg.content.value = i + 100;
        msg_send(&msg, pid_low);   // Send message to low priority thread
        printf("Main thread: Sent message %d to low priority thread\n", i + 100);
    }

    return 0;
}