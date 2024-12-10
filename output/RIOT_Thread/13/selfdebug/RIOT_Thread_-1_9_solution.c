#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE         (THREAD_STACKSIZE_MAIN)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

msg_t message_queue[8];
msg_t msg;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // example value
    while (1) {
        printf("Sender: sending message with value %ld\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        thread_sleep(2);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(message_queue, 8);
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: received message with value %ld\n", msg.content.value);
        }
        thread_sleep(2);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER, CREATE_STACKTEST, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER, CREATE_STACKTEST, receiver, NULL, "receiver");

    // The main thread does not terminate
    while (1) {
        thread_sleep(10);
    }
    return 0;
}