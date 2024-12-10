#include "esp_wifi.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "dht.h"
#include "dht_params.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "kernel_defines.h"

#define SENDER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define NORMAL_PRIORITY (THREAD_PRIORITY_MAIN)

msg_t msg_queue[8];
static char sender_stack[SENDER_STACK_SIZE];
static char receiver_stack[RECEIVER_STACK_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Sample message

    while (1) {
        // Send the message to receiver thread
        if (msg_send(&msg, thread_getpid()) == 1) {
            puts("Message sent successfully.");
        } else {
            puts("Failed to send message.");
        }
        // Use a timer here if needed to repeat sending at intervals
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive messages
        msg_receive(&msg);
        printf("Message received with content: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    // Create threads
    kernel_pid_t sender_pid = thread_create(sender_stack, SENDER_STACK_SIZE,
                                            NORMAL_PRIORITY, 0, sender_thread,
                                            NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, RECEIVER_STACK_SIZE,
                                              HIGH_PRIORITY, 0, receiver_thread,
                                              NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}