#include <xtimer.h>  // Required for timer functions
#include <thread.h>  // Required for thread functionalities
#include <msg.h>     // Required for message APIs
#include <stdio.h>   // Required for standard I/O operations

#define SENDER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 1)

static char sender_stack[SENDER_THREAD_STACKSIZE];
static char receiver_stack[RECEIVER_THREAD_STACKSIZE];

msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    msg.content.value = 42;  // Sample message content

    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);  // Sleep for 1 second before sending the next message
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for the main thread (receiver)
    msg_init_queue(msg_queue, 8);

    // Create sender thread
    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    // Create receiver thread
    thread_create(receiver_stack, sizeof(receiver_stack),
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    // Keep main thread alive
    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}