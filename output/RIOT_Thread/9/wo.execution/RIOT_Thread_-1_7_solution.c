#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "xtimer.h"
#include "dht_params.h"
#include "mbox.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "stdio.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "thread.h"
#include "time_units.h"
#include "net/emcute.h"
#include "dht.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_MAIN  (1024)  // Main thread stack size
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN + 1)  // Main thread priority
#define NUM_MSGS               (8)    // Number of messages

typedef struct {
    uint8_t data;
} message_data_t;

// Function prototypes
tatic void *sender_thread(void *arg);
static void *receiver_thread(void *arg);

int main(void) {
    msg_t msg_queue[NUM_MSGS];
    msg_init_queue(msg_queue, NUM_MSGS);

    kernel_pid_t sender_pid, receiver_pid;

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN - 1, 0,
                               sender_thread, NULL, "sender_thread");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN, 0,
                                 receiver_thread, NULL, "receiver_thread");

    while(1) {
        // Main thread can perform other tasks
        xtimer_sleep(1);
    }

    return 0;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Setting message type, can be any value you need
    msg.content.value = 123;  // Arbitrary value
    printf("Sender: Sending message...\n");
    msg_send(&msg, thread_getpid_of(receiver_thread));
    printf("Sender: Message sent.\n");

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Got message with type %u and value %lu\n", msg.type, msg.content.value);
    }

    return NULL;
}