#include <ztimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <thread.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SENDER_PRIO      (THREAD_PRIORITY_MAIN - 1)

#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)

#define MSG_QUEUE_SIZE     (8) // Must be a power of two

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    while(1) {
        msg_send(&msg, msg_receive());  // Simplified message send
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for some time
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, 0, sender_thread, NULL, "sender");

    // Avoid exiting from main to keep threads running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}