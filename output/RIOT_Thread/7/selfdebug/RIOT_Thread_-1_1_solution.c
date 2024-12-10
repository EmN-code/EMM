#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

// Message queue for the receiver thread
enum { QUEUE_SIZE = 8 };
static msg_t msg_queue[QUEUE_SIZE];

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

// Threads
static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

// The receiver thread function
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, QUEUE_SIZE);
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

// The sender thread function
void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_receive(&msg, &msg, receiver_pid);
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        msg.content.value = i;
        printf("Sending message with value: %d\n", i);
        msg_send(&msg, receiver_pid);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver, NULL, "receiver");
    
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER, 0,
                               sender, NULL, "sender");

    return 0;
}