#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)

static msg_t msg_queue[8];

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    for (int i = 0; i < 5; i++) {
        printf("Sending message: %d\n", i);
        msg.content.value = i;
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Message Passing Example");

    msg_init_queue(msg_queue, 8);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), 
                                            THREAD_PRIORITY_SENDER, 0, 
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                              THREAD_PRIORITY_RECEIVER, 0, 
                                              receiver_thread, NULL, "receiver");

    if (sender_pid <= KERNEL_PID_UNDEF || receiver_pid <= KERNEL_PID_UNDEF) {
        puts("[Error] Failed to create thread(s)");
        return 1;
    }
    return 0;
}