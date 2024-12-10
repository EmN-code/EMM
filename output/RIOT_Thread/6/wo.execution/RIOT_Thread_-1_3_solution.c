#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

msg_t msg_queue_sender[4];
msg_t msg_queue_receiver[4];

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *sender(void *arg) {
    (void)arg; // Avoid unused parameter warning
    msg_t msg;
    msg.content.value = 42;
    printf("Sending message with value: %d\n", msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

static void *receiver(void *arg) {
    (void)arg; // Avoid unused parameter warning
    msg_t msg;
    if (msg_receive(&msg)) {
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Inter-thread Communication Example");

    msg_init_queue(msg_queue_sender, 4);
    msg_init_queue(msg_queue_receiver, 4);

    kernel_pid_t pid_sender = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender, NULL, "sender");

    kernel_pid_t pid_receiver = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN - 2, 0,
                                              receiver, NULL, "receiver");

    return 0;
}