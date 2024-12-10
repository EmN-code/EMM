# Description: This code implements a message box functionality using RIOT OS on ESP32. Messages are sent at regular intervals using a timer.

# Assume this code is used to setup message box and send periodic events

#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "arpa/inet.h"
#include "timex.h"
#include "dht.h"
#include "net/gcoap.h"

#define INTERVAL (1U * US_PER_SEC)

static msg_t queue[8];

static void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    kernel_pid_t pid = thread_create((char *)thread_handler, 1024, THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "thread_handler");
    
    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        msg_send(&msg, pid);
        printf("Message sent with type: %u\n", msg.type);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}