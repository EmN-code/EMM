#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "mutex.h"
#include "msg.h"
#include "mqtt.h"
#include "mbox.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"

#define THREAD_STACKSIZE_MAIN 1024  // Adjust stack size as needed
#define SENDER_PRIORITY       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY     (THREAD_PRIORITY_MAIN + 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8]; // Must be power of two
kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary data
    while (1) {
        if (msg_send(&msg, receiver_pid) <= 0) {
            printf("Failed to send message\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start receiver thread\n");
        return 1;
    }

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start sender thread\n");
        return 1;
    }

    return 0;
}