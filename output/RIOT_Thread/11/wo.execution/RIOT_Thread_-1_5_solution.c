#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "mqttclient.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "irq.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "arpa/inet.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];

msg_t queue[8];

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    
    msg_init_queue(queue, 8);
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = (kernel_pid_t)(intptr_t)arg;

    for (unsigned i = 0; i < 10; ++i) {
        msg.type = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message with type: %u\n", msg.type);
        usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack),
        RECEIVER_THREAD_PRIO, 0,
        receiver, NULL, "receiver"
    );

    thread_create(
        sender_stack, sizeof(sender_stack),
        SENDER_THREAD_PRIO, 0,
        sender, (void *)(intptr_t)receiver_pid, "sender"
    );

    return 0;
}
