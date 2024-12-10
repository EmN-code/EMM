#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include <time_units.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <thread.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h> // Avoid if using RIOT native threads

#define STACKSIZE THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define MESSAGE_QUEUE_SIZE 8

// Thread stacks
char thread_stack_one[STACKSIZE];
char thread_stack_two[STACKSIZE];

// Message queue
static msg_t message_queue[MESSAGE_QUEUE_SIZE];

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (true) {
        printf("Thread One: Sending message\n");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);  // simulate doing something time-consuming
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    while (true) {
        printf("Thread Two: Waiting for message\n");
        msg_receive(&msg);
        printf("Thread Two: Received message with value %lu\n", (unsigned long) msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN, 0, thread_one, NULL, "thread_one");
    thread_create(thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_MAIN, 0, thread_two, NULL, "thread_two");

    // Let threads run
    while (1) {
        // main thread can perform background tasks or sleep
        xtimer_sleep(10);
    }

    return 0;
}