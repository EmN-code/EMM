
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
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define THREAD_MSG_QUEUE_SIZE     (8U)
#define MAIN_QUEUE_SIZE           (4)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message in low priority thread: %ld\n", msg.content.value);
            xtimer_sleep(2);
        }
    }

    return NULL;
}

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    while (1) {
        msg.content.value = (uint32_t)esp_random();  // Use random values to simulate sensor data
        msg.type = 1;

        printf("Sending message from high priority thread: %ld\n", msg.content.value);

        if (msg_send_receive(&msg, &msg, thread_getpid_of(thread_low)) != 1) {
            printf("Failed to send message from high priority thread\n");
        }

        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t high_pid, low_pid;
    char high_thread_stack[THREAD_MSG_QUEUE_SIZE];
    char low_thread_stack[THREAD_MSG_QUEUE_SIZE];

    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    low_pid = thread_create(low_thread_stack, sizeof(low_thread_stack),
                            THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST,
                            thread_low, NULL, "low");

    high_pid = thread_create(high_thread_stack, sizeof(high_thread_stack),
                             THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST,
                             thread_high, NULL, "high");

    (void) high_pid;
    (void) low_pid;

    return 0;
}