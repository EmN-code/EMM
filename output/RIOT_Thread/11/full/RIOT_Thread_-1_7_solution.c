#include "ztimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "esp_system.h"
#include "mqttclient.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "mbox.h"

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define QUEUE_SIZE               8

msg_t queue[QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 42;  // Example message type

    while (1) {
        printf("Sender: sending message...\n");
        msg.content.value = esp_random();  // Send a random value
        if (msg_send_receive(&msg, NULL, RECEIVER_THREAD_PRIORITY) == 1) {
            printf("Sender: message successfully sent!\n");
        } else {
            printf("Sender: message send failed!\n");
        }
        ztimer_sleep(ZTIMER_USEC, 1000000);  // Sleep for 1 second
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: received message with value %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}