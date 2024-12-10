#include "esp_wifi.h"
#include "esp_system.h"
#include "sys/socket.h"

#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <msg.h>
#include <xtimer.h>
#include <dht_params.h>
#include <mbox.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <stdio.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <thread.h>
#include <time_units.h>
#include <net/emcute.h>
#include <dht.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <evtimer_msg.h>

#define STACK_SIZE        (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t receiver_pid;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    /* configure message */
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        /* Send message to the receiver thread */
        msg_send(&msg, receiver_pid);

        /* Pause for a moment */
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 8);  // Queue size of 8, power of 2

    while (1) {
        /* Wait for message */
        msg_receive(&msg);

        /* Print received message type and content */
        printf("Received message: type=%u, value=%u\n", msg.type, msg.content.value);
    }
}

int main(void) {
    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    /* Start the receiver thread */
    receiver_pid = thread_create(receiver_stack, STACK_SIZE,
                                 THREAD_PRIORITY, 0,
                                 receiver_thread, NULL, "receiver");

    /* Start the sender thread */
    thread_create(sender_stack, STACK_SIZE,
                  THREAD_PRIORITY, 0,
                  sender_thread, NULL, "sender");

    /* The main thread needs to be alive to keep the process running */
    while (1) {
        xtimer_sleep(1);
}
}