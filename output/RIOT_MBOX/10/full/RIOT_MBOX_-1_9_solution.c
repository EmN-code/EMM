#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN)
#define MSG_QUEUE_SIZE        8

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_pid;

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = xtimer_now_usec();
        printf("Sending message with timestamp %lu\n", (unsigned long)msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    sender_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, 
                               THREAD_PRIORITY_MAIN + 1, 0, 
                               msg_sender, NULL, "msg_sender");

    msg_t msg;
    while (true) {
        msg_receive(&msg);
        printf("Received message with timestamp %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}