#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
define PRIORITY_LOW  (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;  // Just an example value
    
    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
}

void receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
}

int main(void) {
    // Initialize the message queue
    msg_t rcv_queue[8];
    msg_init_queue(rcv_queue, 8);

    // Create sender and receiver threads
    receiver_pid = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH,
                                 THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    
    sender_pid = thread_create(stack2, sizeof(stack2), PRIORITY_LOW,
                               THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    printf("Threads created successfully\n");

    // Return 0 for successful execution
    return 0;
}