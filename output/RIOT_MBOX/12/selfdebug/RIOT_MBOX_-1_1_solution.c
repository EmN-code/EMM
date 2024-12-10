#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define SENDER_INTERVAL    (1000000U) // 1 second in microseconds
#define MSG_QUEUE_SIZE     (8)

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;
static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Block and wait for a message
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0;

    while (1) {
        // Send a message to the receiver thread
        msg_send(&msg, receiver_pid);
        printf("Sent message\n");
        xtimer_usleep(SENDER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create receiver thread
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, 0, receiver_thread,
                                 NULL, "receiver");

    // Create sender thread
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender_thread,
                               NULL, "sender");

    return 0;
}