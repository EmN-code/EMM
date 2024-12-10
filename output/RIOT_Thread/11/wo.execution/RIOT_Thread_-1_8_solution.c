#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "thread.h"
#include "msg.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "mqtt.h"
#include "mutex.h"
#include "msg_receive.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "irq.h"
#include "mqttclient.h"
#include "malloc.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "msg_reply.h"

// Function prototypes
static void *thread_1(void *arg);
static void *thread_2(void *arg);

// Stack size for threads
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

// Message queue
#define QUEUE_SIZE 8
static msg_t msg_queue[QUEUE_SIZE];

// Thread functions and parameters
char stack_1[THREAD_STACKSIZE];
char stack_2[THREAD_STACKSIZE];

static void *thread_1(void *arg) {
    msg_t msg;
    msg.content.value = *((int*)arg);
    while (1) {
        msg_send(&msg, thread_getpid_of((kernel_pid_t*)thread_2));
        xtimer_sleep(1);
    }
    return NULL;
}

static void *thread_2(void *arg) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    int value = 42;

    thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1, 
                  CREATE_STACKTEST, thread_1, &value, "thread_1");

    thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2, 
                  CREATE_STACKTEST, thread_2, NULL, "thread_2");

    return 0;
}