#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define THREAD_PRIO_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

msg_t msg_queue[8];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        // Receive message
        msg_receive(&msg);

        // Do something with the message
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        // Prepare message
        msg.content.value = 42;  // Arbitrary value

        // Send message
        msg_send(&msg, thread_get_last());

        // Wait for some time before sending the next message
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    // Create receiver thread
    kernel_pid_t pid_receiver = thread_create(receiver_stack, STACKSIZE, THREAD_PRIO_RECEIVER, 0, receiver, NULL, "receiver");

    // Create sender thread
    kernel_pid_t pid_sender = thread_create(sender_stack, STACKSIZE, THREAD_PRIO_SENDER, 0, sender, NULL, "sender");

    // Do something else in main if needed
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}