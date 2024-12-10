#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];

msg_t queue[8];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42;  // example message content
        printf("Sender: Sending message.\n");
        msg_send(&msg, thread_getpid());  // assuming we have a way to get receiver's PID
        ztimer_sleep(ZTIMER_MSEC, 1000);  // sleep for a second
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %ld.\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(sender_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(receiver_stack, STACKSIZE, THREAD_PRIORITY_MAIN, 0, receiver_thread, NULL, "receiver");
    
    return 0;
}