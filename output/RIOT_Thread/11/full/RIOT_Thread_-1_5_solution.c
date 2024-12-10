#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "xtimer.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "pthread.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "thread.h"

#define THREAD_STACKSIZE_MAIN 1024
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void* sender_thread(void* arg) {
    (void)arg;  // To prevent unused warning
    msg_t message;
    message.type = 1;
    message.content.value = 42;
    int target_pid = 2;  // Assume this as a placeholder, real PID will be from actual thread creation

    if(msg_send(&message, target_pid) == 1) {
        printf("Message sent\n");
    }
    else {
        printf("Message sending failed\n");
    }
    return NULL;
}

void* receiver_thread(void* arg) {
    (void)arg;  // To prevent unused warning
    msg_t message;
    msg_receive(&message);
    printf("Received message of type: %u and value: %lu\n", message.type, (unsigned long)message.content.value);
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    pthread_t sender;
    pthread_t receiver;

    pthread_create(&sender, NULL, sender_thread, NULL);
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}