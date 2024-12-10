#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10
#define MSG_QUEUE_SIZE 8

char thread_sender_stack[THREAD_STACKSIZE_MAIN];
char thread_receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42; // Example message content
        msg_send(&msg, thread_getpid());
        printf("Message sent with value: %d\n", msg.content.value);
        // Sleep to allow other threads to execute
        thread_yield();
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_sender_stack, sizeof(thread_sender_stack),
                  THREAD_PRIORITY_LOW, 0, sender, NULL, "sender");
    
    thread_create(thread_receiver_stack, sizeof(thread_receiver_stack),
                  THREAD_PRIORITY_HIGH, 0, receiver, NULL, "receiver");

    return 0;
}