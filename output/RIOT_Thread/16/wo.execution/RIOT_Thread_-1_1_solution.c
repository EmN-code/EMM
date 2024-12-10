#include <thread.h>
#include <msg.h>
#include <sched.h>
#include <stdio.h>
#include <xtimer.h>

// Priority levels (lower numbers indicate higher priority)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 1)

#define SENDER_QUEUE_SIZE (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t sender_queue[SENDER_QUEUE_SIZE];

// Define message variable
static msg_t msg;

void *receiver_thread(void *arg) {
    (void)arg;
    printf("Receiver thread started!\n");

    while (1) {
        msg_receive(&msg);
        printf("Message received with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = thread_getpid(); // Assuming receiver thread is already running.

    printf("Sender thread started!\n");

    while (1) {
        msg.content.value = 42;  // Arbitrary value for demonstration
        printf("Sending message...\n");
        msg_send(&msg, receiver_pid);

        // Sleep for 2 seconds
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    // Initiate message queue
    msg_init_queue(sender_queue, SENDER_QUEUE_SIZE);

    // Create threads
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_RECEIVER, 0, receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");
    return 0;
}