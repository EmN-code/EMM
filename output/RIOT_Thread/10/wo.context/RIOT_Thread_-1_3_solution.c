#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN)

char sender_stack[SENDER_STACKSIZE];
char receiver_stack[RECEIVER_STACKSIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 123;  // Example message content
    while (1) {
        printf("Sender: Sending message containing: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid_of(receiver_stack));
        xtimer_sleep(2);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with content: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("Starting sender and receiver threads.\n");
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");
    return 0;
}