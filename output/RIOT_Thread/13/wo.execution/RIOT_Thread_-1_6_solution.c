#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN 1024 // Define main thread stack size
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1) // Adjust main thread priority

// Thread stack allocations
char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];

// Message queues
msg_t thread_one_queue[8];
msg_t thread_two_queue[8];

// Thread functions
void* thread_one(void* arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message
    while (1) {
        msg_send(&msg, thread_getpid() + 1); // Assume thread_two is the next PID
        ztimer_sleep(ZTIMER_USEC, 1000000);    // 1 second delay
    }
    return NULL;
}

void* thread_two(void* arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queues
    msg_init_queue(thread_one_queue, ARRAY_SIZE(thread_one_queue));
    msg_init_queue(thread_two_queue, ARRAY_SIZE(thread_two_queue));

    // Create threads
    thread_create(thread_one_stack, sizeof(thread_one_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_create(thread_two_stack, sizeof(thread_two_stack), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    return 0;
}