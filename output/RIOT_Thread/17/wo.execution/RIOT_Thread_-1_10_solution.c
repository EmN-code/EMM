#include <esp_system.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <thread.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <mutex.h>

#define SENDER_PRIO    (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO  (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;

    for (int i = 0; i < 10; i++) {
        msg.type = i;
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message with type: %d and value: %d\n", msg.type, msg.content.value);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d and value: %d\n", msg.type, msg.content.value);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, &receiver_pid, "sender");

    return 0;
}