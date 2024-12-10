#include <stdio.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 3)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while (1) {
        msg.content.value = counter++;
        printf("Sending message: %ld\n", msg.content.value);
        msg_send(&msg, thread_get(RECEIVER_PRIO));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");

    return 0;
}