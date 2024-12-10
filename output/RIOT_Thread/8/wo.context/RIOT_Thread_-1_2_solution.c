#include <stdio.h>
#include <net/sock/udp.h>
#include <net/sock/dtls.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN        (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN         (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_SENDER       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER     (THREAD_PRIORITY_MAIN - 2)
#define QUEUE_SIZE                    (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t sender_queue[QUEUE_SIZE];
msg_t receiver_queue[QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 123;
    while (1) {
        msg_send(&msg, thread_getpid()); // Send to self for testing
        msg_send(&msg, thread_get(THREAD_PRIORITY_RECEIVER)); // Send to receiver
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(sender_queue, QUEUE_SIZE);
    msg_init_queue(receiver_queue, QUEUE_SIZE);

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}