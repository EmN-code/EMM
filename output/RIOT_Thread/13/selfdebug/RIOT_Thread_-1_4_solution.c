#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue_sender[MSG_QUEUE_SIZE];
static msg_t msg_queue_receiver[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message payload
    printf("Sender: Sending message with value %u\n", (unsigned int)msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue_receiver, MSG_QUEUE_SIZE);
    printf("Receiver: Waiting for message...\n");
    if (msg_receive(&msg) == 1) {
        printf("Receiver: Received message with value %u\n", (unsigned int)msg.content.value);
        msg_reply(&msg, &msg); // Use correct arguments types
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "Receiver");

    thread_create(NULL, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "Sender");

    while (1) {
        // Main loop does nothing, just demonstrates threads running
    }
    return 0;
}