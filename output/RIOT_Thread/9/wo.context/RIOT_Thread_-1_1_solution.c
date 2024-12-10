#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define STACK_SIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack_one[STACK_SIZE];
static char stack_two[STACK_SIZE];

static kernel_pid_t thread_one_pid;
static kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    msg_t msg;
    msg.content.value = 42;  // Just an example

    while (1) {
        printf("Thread One: Sending message with value %ld\n", msg.content.value);
        msg_send(&msg, thread_two_pid);
        thread_yield();
    }
    return NULL;
}

void *thread_two(void *arg) {
    msg_t msg;

    while (1) {
        if (msg_try_receive(&msg) == 1) {
            printf("Thread Two: Received message with value %ld\n", msg.content.value);
        } else {
            printf("Thread Two: No message received, continuing\n");
        }
        thread_yield();
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_one_pid = thread_create(stack_one, STACK_SIZE, PRIORITY_LOW, 0, thread_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack_two, STACK_SIZE, PRIORITY_HIGH, 0, thread_two, NULL, "thread_two");

    printf("Threads have been started\n");

    return 0;
}