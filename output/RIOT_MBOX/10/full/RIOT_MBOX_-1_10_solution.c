#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "string.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
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

// Description:
// This program on RIOT OS for ESP32 demonstrates message queue functionality.
// It creates a thread that periodically sends messages using a message queue.
// To resolve the errors, ensure not to redefine THREAD_STACKSIZE_MAIN and THREAD_PRIORITY_MAIN.
// This program will make sure the constants are not redefined and utilize proper headers provided.

#define MAIN_QUEUE_SIZE     (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static char stack[THREAD_STACKSIZE_MAIN];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    while (1) {
        // Send message to self, simulating message box functionality
        msg_send(&msg, thread_getpid());
        printf("Message sent with value: %u\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    // Initialization of the main message queue
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    // Create a thread for sending messages
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, msg_sender, NULL, "msg_sender");

    // Main thread loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread received message with value: %u\n", msg.content.value);
    }
    return 0;
}