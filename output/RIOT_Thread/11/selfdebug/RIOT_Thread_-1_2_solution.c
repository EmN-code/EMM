#include <thread.h>
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg;
msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        msg.content.value = 42;
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        printf("Sent message with value: %d\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, thread_getpid_of(receiver_stack), "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    /* Let the main thread sleep to lower power consumption */
    xtimer_sleep(1);
    
    return 0;
}
