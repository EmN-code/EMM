#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "string.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "stddef.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "mutex.h"
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
#include "esp_system.h"
#include "dht.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#define MBOX_SIZE 8

static mbox_t my_mbox;
static msg_t mbox_queue[MBOX_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = rand();  // Generate a random integer to send
        if (mbox_try_send(&my_mbox, &msg) == 0) {
            printf("Message sent: %u\n", msg.content.value);
        } else {
            printf("Failed to send message\n");
        }
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&my_mbox, &msg);
        printf("Message received: %u\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, mbox_queue, MBOX_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}