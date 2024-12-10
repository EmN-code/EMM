#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "thread.h"
#include "timex.h"
#include "unistd.h"
#include "sys/socket.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mbox.h"
#include "stdatomic.h"
#include "dht.h"
#include "dht_params.h"
#include "netinet/in.h"
#include "mma8x5x_regs.h"

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    kernel_pid_t pid = *((kernel_pid_t *)arg);

    while (1) {
        if (msg_send_receive(&msg, &msg, pid) != 1) {
            printf("Failed to send message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) != 1) {
            printf("Failed to receive message\n");
        } else {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);

    kernel_pid_t sender_pid;
    kernel_pid_t receiver_pid;

    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
        THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(
        sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
        THREAD_CREATE_STACKTEST, sender_thread, &receiver_pid, "sender");

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}