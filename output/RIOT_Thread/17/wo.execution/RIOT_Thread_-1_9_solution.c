#include <sys/socket.h>
#include <ztimer.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <mutex.h>
#include <msg.h>
#include <time_units.h>
#include <thread.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <net/sock/util.h>
#include <dht.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <timex.h>
#include <mma8x5x.h>
#include <unistd.h>
#include <mma8x5x_params.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

msg_t msg_queue[8];

void *sender(void *arg) {
    msg_t msg;
    msg.content.value = *(uint32_t *)arg;
    printf("Sender: Sending message with value: %lu\n", msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Receiver: Received message with value: %lu\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    char stack_sender[STACKSIZE];
    char stack_receiver[STACKSIZE];

    uint32_t sender_value = 12345;

    thread_create(stack_sender, STACKSIZE, THREAD_PRIORITY_MAIN, 0,
                  sender, &sender_value, "sender");

    thread_create(stack_receiver, STACKSIZE, THREAD_PRIORITY_MAIN, 0,
                  receiver, NULL, "receiver");

    while (1) {
        thread_yield();
    }

    return 0;
}