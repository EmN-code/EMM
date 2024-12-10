#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "time_units.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "mbox.h"
#include "mutex.h"

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN    (8)
#define THREAD_PRIORITY_SENDER  (7)
#define THREAD_PRIORITY_RECEIVER (6)

msg_t msg_queue[8];

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    unsigned int type = 1;
    while (1) {
        msg.type = type;
        msg.content.value = thread_getpid();
        msg_send(&msg, thread_getpid_of(&receiver_thread));
        printf("Sent message with type: %u by thread: %d\n", msg.type, msg.content.value);
        xtimer_sleep(1);
        type++;
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        msg_receive(&msg);
        printf("Received message with type: %u by thread: %lu\n", msg.type, (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_thread.pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                        THREAD_PRIORITY_RECEIVER, 0,
                                        receiver, NULL, "receiver");
    
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender, NULL, "sender");

    msg_init_queue(msg_queue, 8);

    while (1) {
        // Main thread can perform other tasks or sleep
        xtimer_sleep(1);
    }

    return 0;
}