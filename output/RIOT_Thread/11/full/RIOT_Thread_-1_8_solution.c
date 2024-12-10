#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "mutex.h"
#include "unistd.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "arch/architecture.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "pthread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        printf("Sending message...\n");
        msg_send_receive(&msg, NULL, thread_getpid());
        xtimer_sleep(1); // Send a message every second
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    while (1) {
        // Main loop can be used for other tasks
        xtimer_sleep(1);
    }

    return 0;
}