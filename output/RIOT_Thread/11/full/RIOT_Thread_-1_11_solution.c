#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ztimer.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "architecture.h"
#include "kernel_defines.h"
#include "timex.h"
#include "xtimer.h"
#include "mbox.h"
#include "mutex.h"
#include "event/thread.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define STACK_SIZE (1024)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN + 1)

static char stack_one[STACK_SIZE];
static char stack_two[STACK_SIZE];

static msg_t queue[8];
static kernel_pid_t main_thread_pid;

void *thread_one(void *arg) {
    msg_t msg;
    msg.type = 0x101;
    msg.content.value = 42;
    
    printf("Thread One: Sending message.\n");
    msg_send(&msg, main_thread_pid);
    return NULL;
}

void *thread_two(void *arg) {
    msg_t msg;
    
    printf("Thread Two: Waiting for message.\n");
    msg_receive(&msg);
    printf("Thread Two: Received message with value: %u\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));
    main_thread_pid = thread_getpid();
    
    thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN,
                  CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_SECOND,
                  CREATE_STACKTEST, thread_two, NULL, "thread_two");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("ESP32 RIOT OS Application Running\n");
    }
    return 0;
}