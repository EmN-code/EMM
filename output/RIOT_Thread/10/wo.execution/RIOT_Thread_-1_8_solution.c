#include "esp_system.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "msg.h"

#define MAIN_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN + 1)
#define MAIN_STACKSIZE         (THREAD_STACKSIZE_MAIN)

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 2)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 3)
#define THREAD_STACKSIZE       (THREAD_STACKSIZE_MAIN)

msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        printf("Sender: Sending a message to receiver thread.\n");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received a message from sender thread.\n");
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    char sender_stack[THREAD_STACKSIZE];
    char receiver_stack[THREAD_STACKSIZE];

    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
