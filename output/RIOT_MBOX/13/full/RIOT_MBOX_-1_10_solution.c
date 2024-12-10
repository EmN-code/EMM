// Required header files
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stdatomic.h"
#include "kernel_defines.h"

// Define message structure
typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

#define PRIORITY_MAIN    (32)
#define STACKSIZE_MAIN   (KERNEL_CONF_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE   (8)

void msg_init_queue(msg_t *array, int num);

int msg_send_receive(msg_t *m, msg_t *reply, kernel_pid_t target_pid);

void sender_thread(void *arg) {
    msg_t msg;
    msg.sender_pid = thread_getpid();
    msg.type = 0;
    msg.content.value = 42;  // example message content
    
    printf("Sender: Sending message with value %u\n", msg.content.value);
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);

    // Send message
    msg_t reply;
    while (msg_send_receive(&msg, &reply, receiver_pid) != 1) {
        printf("Sender: Retrying to send message\n");
        // Sleep or use a delay mechanism if necessary to avoid busy-waiting
    }

    printf("Sender: Message sent successfully\n");
}

void receiver_thread(void) {
    msg_t msg;
    msg_init_queue(&msg, MSG_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value %u\n", msg.content.value);
            // Process received message
            msg_t reply;
            reply.type = msg.type;
            reply.content.value = msg.content.value + 1;  // Example processing
            msg_reply(&msg, &reply);
        }
    }
}

int main(void) {
    msg_t main_msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(main_msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t receiver_pid;

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY_MAIN,
                                 CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid;
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), PRIORITY_MAIN,
                               CREATE_STACKTEST, sender_thread, &receiver_pid, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create sender thread\n");
        return 1;
    }

    thread_yield();  // Yield to allow created threads to run

    return 0;
}