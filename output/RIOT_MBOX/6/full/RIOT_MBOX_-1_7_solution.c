#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL_USEC (1000000) // 1 second in microseconds

static char sender_stack[SENDER_THREAD_STACKSIZE];
static kernel_pid_t sender_pid;

static void *sender_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg_send(&msg, thread_getpid());
        printf("Sent message with value: %lu\n", msg.content.value);
        msg.content.value++;
        ztimer_sleep(ZTIMER_USEC, INTERVAL_USEC);
    }

    return NULL;
}

int main(void) {
    msg_t main_msg_queue[8];
    msg_init_queue(main_msg_queue, 8);

    sender_pid = thread_create(
        sender_stack,
        sizeof(sender_stack),
        SENDER_THREAD_PRIORITY,
        0,
        sender_function,
        NULL,
        "sender_thread"
    );

    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error initializing sender thread.");
        return 1;
    }

    puts("Message box example started");
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return 0;
}