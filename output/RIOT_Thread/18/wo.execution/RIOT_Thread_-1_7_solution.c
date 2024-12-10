// Description: This code demonstrates inter-thread communication in RIOT OS using msg_t structure. It creates two threads with different priorities, and one thread sends messages to another. 

// Import necessary RIOT OS header files
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "thread.h"
#include "msg.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define MESSAGE_QUEUE_SIZE (8)

// Thread stack and message queue
char thread_stack_sender[THREAD_STACKSIZE_MAIN];
char thread_stack_receiver[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[MESSAGE_QUEUE_SIZE];

// Function prototypes
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

// Shared structure to send messages
typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} shared_msg_t;

// Sender thread
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    shared_msg_t *shared_msg = (shared_msg_t*)&msg;
    
    printf("Sender thread started!\n");
    
    for (uint32_t value = 0; value < 5; ++value) {
        shared_msg->sender_pid = thread_getpid();
        shared_msg->type = 1;
        shared_msg->content.value = value;

        // Send message to receiver thread
        if (msg_try_send(&msg, thread_getpid() - 1) == 1) {
            printf("Message with value %lu sent.\n", (unsigned long)shared_msg->content.value);
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

// Receiver thread
void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    shared_msg_t *shared_msg;

    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    printf("Receiver thread started!\n");

    while (1) {
        msg_receive(&msg);
        shared_msg = (shared_msg_t*)&msg;
        printf("Received message with value %lu from PID %u\n", (unsigned long)shared_msg->content.value, (unsigned int)shared_msg->sender_pid);
    }
    return NULL;
}

int main(void) {
    // Create sender and receiver threads
    thread_create(thread_stack_sender, sizeof(thread_stack_sender),
                  SENDER_PRIO, 0, sender_thread, NULL, "sender");
    thread_create(thread_stack_receiver, sizeof(thread_stack_receiver),
                  RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");

    return 0;
}