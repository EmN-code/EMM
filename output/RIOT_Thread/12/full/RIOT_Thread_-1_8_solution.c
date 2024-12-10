#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "time_units.h"
#include "timex.h"
#include "net/sock/util.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "thread.h"
#include "stdatomic.h"

#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE  (THREAD_STACKSIZE_MAIN)
#define MSG_TYPE_EVENT_START (0xABCD) // Custom message type

static char thread_stack[THREAD_STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void) arg;
    
    msg_t msg;
    msg.type = MSG_TYPE_EVENT_START;
    msg.content.value = 42;  // Example payload value

    while (1) {
        msg_send(&msg, receiver_pid);
        dht_measure(dht_t *dev)
        // Sleep for 1 second
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_EVENT_START) {
            printf("Received message with value: %u\n", msg.content.value);
        }

        // Process the message here
    }

    return NULL;
}

int main(void) {
    printf("RIOT OS message passing example\n");

    // Initialize message queue for the receiver
    msg_init_queue(queue, THREAD_STACKSIZE);

    // Create the sender thread
    sender_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, 0, sender_thread, NULL, "sender");

    // Create the receiver thread
    receiver_pid = thread_create(thread_stack, sizeof(thread_stack),
                                 THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");

    // Main loop does nothing; threads are running
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
