#include <msg.h>  // We need msg.h to work with message boxes and for inter-process communication
#include "xtimer.h"
#include <stdio.h>

#define MSG_BUFFER_SIZE (8)  // Define the size of the message buffer

typedef struct {
    int counter;
} event_msg_t;

static msg_t _msg_queue[MSG_BUFFER_SIZE];  // Message queue to store the incoming messages

void sender_thread(void) {
    event_msg_t evt_msg;
    msg_t msg;
    msg.type = 0x100;
    
    while (1) {
        evt_msg.counter++;
        msg.content.ptr = &evt_msg;
        msg_send(&msg, msgbus_get_receiver());
        printf("Message sent with counter: %d\n", evt_msg.counter);
        xtimer_sleep(5);  // Sleep for 5 seconds before sending the next message
    }
}

void receiver_thread(void) {
    msg_t msg;
    event_msg_t *received_msg;

    while (1) {
        msg_receive(&msg);
        received_msg = (event_msg_t *)msg.content.ptr;
        printf("Message received with counter: %d\n", received_msg->counter);
    }
}

int main(void) {
    printf("Welcome to the message box example\n");
    
    msg_init_queue(_msg_queue, MSG_BUFFER_SIZE);  // Initialize the message queue

    // Create threads for sending and receiving messages
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                              THREAD_PRIORITY_MAIN - 2, 0,
                                              receiver_thread, NULL, "receiver");

    // Join threads
    thread_join(sender_pid, NULL);
    thread_join(receiver_pid, NULL);

    return 0;
}
