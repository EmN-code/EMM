#include <thread.h>
#include <stdio.h>

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE 1024

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // example message content
    while(1) {
        msg_send(&msg, thread_get(THREAD_PRIORITY_RECEIVER));
        printf("Sender: Sent message with value %d\n", msg.content.value);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                  0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                  0, sender_thread, NULL, "sender");
    return 0;
}