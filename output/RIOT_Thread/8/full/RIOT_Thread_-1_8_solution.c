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
#include "esp_system.h"
#include "dht_params.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
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

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[8];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent\n");
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;

    receiver_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, CREATE_STACKTEST, sender_thread, &receiver_pid, "sender");

    while (1) {
        thread_yield();
    }
    return 0;
}