#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "pthread.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "irq.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "net/emcute.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void sender(void) {
    msg_t msg;
    msg.type = 1;
    msg_send(&msg, thread_getpid());
}

void receiver(void) {
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with type: %u\n", msg.type);
}

int main(void) {
    msg_init_queue(msg_queue, 8); // Initialize message queue for inter-thread messages

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, receiver, NULL, "receiver");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error in creating the receiver thread\n");
        return 1;
    }

    sender();

    return 0;
}
