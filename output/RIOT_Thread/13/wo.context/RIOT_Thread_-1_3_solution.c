#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>  // Assuming we might need functionalities for standard I/O operations in the future
#include <thread.h>

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

msg_t queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = rand();  // Assuming we are sending random values
        printf("[SENDER] Sending value: %d\n", msg.content.value);
        msg_send(&msg, thread_get(receiver_stack));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("[RECEIVER] Received value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(receiver_stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_RECEIVER, 0, receiver, NULL, "receiver");

    thread_create(sender_stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_SENDER, 0, sender, NULL, "sender");

    return 0;
}