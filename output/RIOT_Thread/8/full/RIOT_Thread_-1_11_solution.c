#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "atomics.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "kernel_defines.h"
#include "dht.h"
#include "dht_params.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "xtimer.h"
#include "timex.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"
#include "mbox.h"
#include "thread.h"

#define THREAD_PRIORITY_MAIN 5

kernel_pid_t sender_pid;
msg_t msg_queue[8];
msg_t msg;

void sender_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg.content.value = rand();
        msg_send(&msg, sender_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    sender_pid = thread_create((char *)stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create((char *)stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
