#include <thread.h>
#include <sched.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (SCHED_PRIO_MAIN)

// Definition for stack size and message type
#define STACK_SIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
#define MSG_TYPE 1

// Global declaration of thread stacks
char thread_one_stack[STACK_SIZE];
char thread_two_stack[STACK_SIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

// Message queue
msg_t msg_queue[8];

// Thread function for sending messages
void *thread_one_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE;
    msg.content.value = 42; // Example message content

    while (1) {
        printf("Thread One: Sending message with content %d\n", msg.content.value);
        msg_send(&msg, thread_two_pid);
        thread_yield();
    }
    return NULL;
}

// Thread function for receiving messages
void *thread_two_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Thread Two: Received message with content %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("Starting thread communication example\n");

    // Creating thread one and two
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                   thread_one_handler, NULL, "thread_one");
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                   thread_two_handler, NULL, "thread_two");

    return 0;
}