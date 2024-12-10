// To solve the error, we need to ensure the format specifier matches the variable type. The error message indicates that a '%d' format specifier is being used for a 'uint32_t' type.
// We should use '%"PRIu32"' instead of '%d' for printing 'uint32_t'.

#include <inttypes.h>  // For PRIu32
#include <stdio.h>     // For printf

#include "msg.h"
#include "time_units.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "xtimer.h"
#include "net/sock/util.h"
#include "dht.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"

#define STACK_SIZE 1024
#define PRIORITY_MAIN 5
#define PRIORITY_SECONDARY 6

msg_t msg_queue[8];  // message queue must be a power of two

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        printf("Sending message with value: %" PRIu32 "\n", msg.content.value);
        msg_send(&msg, KERNEL_PID_MAIN);
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
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    thread_create(sender_stack, sizeof(sender_stack), PRIORITY_SECONDARY, 0,
                  sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY_MAIN, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}