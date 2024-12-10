#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#include <stdio.h>  // For printf
#include <stdlib.h> // For malloc
#include <xtimer.h> // For xtimer_sleep

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char sender_thread_stack[SENDER_THREAD_STACKSIZE];
char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

void *sender_thread(void *arg) {
    while (1) {
        msg_t msg;
        msg.content.value = (uint32_t)rand();
        printf("Sending message: %lu\n", msg.content.value);

        if (msg_send_receive(&msg, &msg, *((kernel_pid_t *)arg)) == 1) {
            printf("Message sent successfully.\n");
        } else {
            printf("Message sending failed.\n");
        }

        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 1);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message: %lu\n", msg.content.value);
            msg_reply(&msg, &msg);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack,
                                              RECEIVER_THREAD_STACKSIZE,
                                              THREAD_PRIORITY_MAIN - 1,
                                              0,
                                              receiver_thread,
                                              NULL,
                                              "receiver");

    kernel_pid_t sender_pid = thread_create(sender_thread_stack,
                                            SENDER_THREAD_STACKSIZE,
                                            THREAD_PRIORITY_MAIN - 1,
                                            0,
                                            sender_thread,
                                            &receiver_pid,
                                            "sender");

    printf("Sender PID: %d, Receiver PID: %d\n", sender_pid, receiver_pid);

    return 0;
}