#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    int i;
    for (i = 0; i < 10; i++) {
        msg_send(&msg, thread_getpid_of(&receiver_thread));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %u with value: %lu\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    char thread_stack_sender[THREAD_STACKSIZE];
    char thread_stack_receiver[THREAD_STACKSIZE];

    kernel_pid_t sender_pid = thread_create(thread_stack_sender, sizeof(thread_stack_sender),
                                            THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(thread_stack_receiver, sizeof(thread_stack_receiver),
                                              THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    printf("Sender thread PID: %d, Receiver thread PID: %d\n", sender_pid, receiver_pid);

    while (1) {}
    return 0;
}