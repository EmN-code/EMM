#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define STACK_SIZE 1024
#define PRIORITY_MAIN 5
#define PRIORITY_SENDER 3
#define PRIORITY_RECEIVER 2

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

msg_t sender_queue[8];
msg_t receiver_queue[8];

void *sender_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    
    for (int i = 0; i < 5; ++i) {
        msg.content.value = i;
        printf("Sender: Sending message %d\n", i);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT threads example\n");

    // Initialize message queues
    msg_init_queue(sender_queue, sizeof(sender_queue) / sizeof(msg_t));
    msg_init_queue(receiver_queue, sizeof(receiver_queue) / sizeof(msg_t));

    // Create sender thread
    sender_pid = thread_create(sender_stack, STACK_SIZE, PRIORITY_SENDER, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Failed to create sender thread\n");
        return 1;
    }

    // Create receiver thread
    receiver_pid = thread_create(receiver_stack, STACK_SIZE, PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Failed to create receiver thread\n");
        return 1;
    }

    // Run the main thread in an idle loop
    while (1) {
        thread_yield();
    }

    return 0;
}