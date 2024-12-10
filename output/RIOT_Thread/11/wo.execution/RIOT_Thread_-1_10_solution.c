// Description: Implement a RIOT-based inter-thread communication system on ESP32. Due to constraints, ensure to include required libraries and manage priorities while sending messages.

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "msg.h"
#include "thread.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];
msg_t msg;

void *sender(void *arg) {
    (void)arg;
    msg_t message;
    message.type = 1;
    message.content.value = 42;
    
    while (1) {
        printf("Sender: sending message with value %d\n", message.content.value);
        msg_send(&message, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t message;
    
    while (1) {
        msg_receive(&message);
        printf("Receiver: got message with value %d\n", message.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT multi-threading with message passing example\n");
    
    msg_init_queue(msg_queue, 8);
    
    if (thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                      CREATE_STACKTEST, receiver, NULL, "receiver") <= KERNEL_PID_UNDEF) {
        puts("Receiver thread creation failed\n");
        return 1;
    }

    if (thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                      CREATE_STACKTEST, sender, NULL, "sender") <= KERNEL_PID_UNDEF) {
        puts("Sender thread creation failed\n");
        return 1;
    }
    
    return 0;
}