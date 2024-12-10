#include <xtimer.h>          // For using timers with RIOT
#include <stdio.h>            // For standard input/output
#include <pthread.h>          // For threading support
#include <event.h>            // For event queue management
#include <msg.h>              // For message box functionality in RIOT
#include "thread.h"          // For threading abstraction in RIOT

#define MAIN_QUEUE_SIZE     (8) // Size of the message queue
#define INTERVAL            (1000000U) // Interval for sending messages in microseconds

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Thread stack definition
enum { THREAD_STACKSIZE = THREAD_STACKSIZE_MAIN };   
char stack[THREAD_STACKSIZE];    // Allocate thread stack

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Send a message to self after INTERVAL microseconds
        xtimer_usleep(INTERVAL);
        printf("Sending message ...\n");
        msg.content.value = 42;  // Just an arbitrary number for illustration
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Setup and start a thread for message sending
    pthread_t thread;
    pthread_create(&thread, NULL, message_sender, NULL);

    // Message receive loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return 0;
}