#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)

static kernel_pid_t low_priority_thread_pid;
static char high_priority_stack[STACK_SIZE];
static char low_priority_stack[STACK_SIZE];

// Message queues for each thread
msg_t msg_queue[8];

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg, reply;
    while (1) {
        // Prepare a message
        msg.content.value = 42;

        // Send a message to the low priority thread
        if (msg_send_receive(&msg, &reply, low_priority_thread_pid) == 1) {
            printf("High priority thread received reply: %d\n", reply.content.value);
        }
        thread_yield();
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Wait for a message from the high priority thread
        if (msg_receive(&msg) == 1) {
            printf("Low priority thread received message: %d\n", msg.content.value);
            msg_t reply_msg = msg;
            reply_msg.content.value = msg.content.value + 1;
            msg_reply(&msg, &reply_msg);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for the main thread
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    // Start the low priority thread
    low_priority_thread_pid = thread_create(low_priority_stack, STACK_SIZE, PRIORITY_LOW, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    // Start the high priority thread
    thread_create(high_priority_stack, STACK_SIZE, PRIORITY_HIGH, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    return 0;
}