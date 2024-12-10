#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <atomic.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_MAIN 1024
#define MESSAGE_QUEUE_SIZE 4

static mbox_t mbox;
static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_receive(&msg, &msg, thread_getpid());
    while (1) {
        msg.content.value = esp_random();
        if (mbox_try_put(&mbox, &msg) != 1) {
            printf("mbox is full\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    /* Initialize and configure mailbox */
    mbox_init(&mbox, msg_queue, MESSAGE_QUEUE_SIZE);

    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

    /* Create sender and receiver threads */
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}