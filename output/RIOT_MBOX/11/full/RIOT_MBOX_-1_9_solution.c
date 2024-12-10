#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "timex.h"
#include "mbox.h"
#include "thread.h"
#include "msg.h"

#define INTERVAL_MS (1000U)
#define MSG_QUEUE_SIZE (8U)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;    
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        printf("Message sent with value: %u\n", msg.content.value);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received with value: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);    
    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    char receiver_thread_stack[THREAD_STACKSIZE_MAIN];
    
    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}