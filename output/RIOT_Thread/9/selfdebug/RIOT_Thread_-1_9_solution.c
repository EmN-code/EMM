#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_stack[SENDER_STACKSIZE];
char receiver_stack[RECEIVER_STACKSIZE];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    printf("Sender: Sending message...\n");
    msg_send(&msg, receiver_pid);
    return NULL;
}

static void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Receiver: Waiting for message...\n");
    msg_receive(&msg);
    printf("Receiver: Received message with value: %d\n", msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, 
                                              RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, 
                  SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender, (void *)&receiver_pid, "sender");

    return 0;
}