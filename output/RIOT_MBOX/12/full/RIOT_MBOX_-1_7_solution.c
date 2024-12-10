#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <pthread.h>
#include <mutex.h>
#include <unistd.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <dht.h>
#include <timex.h>
#include <time_units.h>
#include <arpa/inet.h>
#include <dht_params.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL                (1000000U) /* Time interval for sending messages in microseconds */

static msg_t msg_queue[8];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg_t reply;
    
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    for (size_t i = 0; i < 5; i++) {
        msg.content.value = i;
        printf("Sending message %zu\n", i);
        if (msg_send_receive(&msg, &reply, receiver_pid) <= 0) {
            printf("Failed to send message\n");
        }
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

void event_handler(msg_t *m) {
    printf("Received message of type %04x with value %u\n", m->type, m->content.value);
}

void *event_receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        event_handler(&msg);
    }
    return NULL;
}

int main(void) {
    puts("RIOT message event example");

    kernel_pid_t receiver_pid = thread_create(
        (char *)msg_queue,
        sizeof(msg_queue),
        THREAD_PRIORITY_MAIN,
        0,
        event_receiver,
        NULL,
        "event_receiver");

    thread_create(
        (char *)msg_queue,
        sizeof(msg_queue),
        THREAD_PRIORITY_MAIN,
        0,
        msg_sender,
        &receiver_pid,
        "msg_sender");

    return 0;
}