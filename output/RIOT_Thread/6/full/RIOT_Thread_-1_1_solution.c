#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "kernel.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE (8)

msg_t queue[MSG_QUEUE_SIZE];

static char thread_one_stack[STACK_SIZE];
static char thread_two_stack[STACK_SIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    int cnt = 0;
    
    while (1) {
        msg.content.value = cnt++;
        msg_reply(&msg, &msg);  // Send message to another thread
        msg_send(&msg, thread_two_pid);
        printf("Thread One: Sent message with value %d\n", msg.content.value);
        thread_sleep(); // Yield control to another thread
    }

    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Thread Two: Received message with value %d\n", msg.content.value);
        }
        thread_sleep(); // Yield control to another thread
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, MSG_QUEUE_SIZE);

    thread_one_pid = thread_create(thread_one_stack, STACK_SIZE, 
                                   THREAD_PRIORITY_MAIN - 1, 
                                   THREAD_CREATE_STACKTEST,
                                   thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, STACK_SIZE, 
                                   THREAD_PRIORITY_MAIN - 2, 
                                   THREAD_CREATE_STACKTEST,
                                   thread_two, NULL, "thread_two");

    /* The main thread needs to keep running so the created threads can execute */
    while (1) {
        thread_sleep();
    }

    return 0;
}