#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <string.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <mutex.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <esp_system.h>
#include <dht.h>
#include <sys/socket.h>
#include <net/gcoap.h>

#ifndef THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#endif

static char stack[THREAD_STACKSIZE_MAIN];

void *message_sender(void *arg) {
    msg_t msg;
    msg.type = 42; // Arbitrary message type
    int counter = 0;

    while (1) {
        printf("Sending message %d\n", counter);
        msg.content.value = counter++;
        msg_send(&msg, thread_getpid());
        xtimer_sleep(5); // Send every 5 seconds
    }
    
    return NULL;
}

int main(void) {
    printf("Message box demo with event sending at intervals.\n");

    msg_t queue[8];
    msg_init_queue(queue, 8);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, message_sender,
                  NULL, "message_sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return 0;
}