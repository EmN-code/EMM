#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 3)
#define MSG_QUEUE_SIZE 8

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;  // Arbitrary value for demonstration
        msg_send(&msg, thread_getpid());
        printf("Message sent: %d\n", msg.content.value);
        xtimer_sleep(1);  // Wait for 1 second before sending the next message
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    return 0;
}