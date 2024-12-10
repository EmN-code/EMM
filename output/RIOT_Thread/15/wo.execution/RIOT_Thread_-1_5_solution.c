#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "msg.h"
#include "thread.h"

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_thread_stack[SENDER_THREAD_STACKSIZE];
char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u, value: %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    ztimer_sleep(ZTIMER_SEC, 2);
    
    msg_send(&msg, thread_getpid_of(receiver_thread));
    printf("Message sent with type: %u, value: %u\n", msg.type, msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE,
                                               RECEIVER_THREAD_PRIORITY, 0,
                                               receiver_thread, NULL, "receiver");
    
    thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE,
                  SENDER_THREAD_PRIORITY, 0,
                  sender_thread, (void *)receiver_pid, "sender");

    return 0;
}