#include <ztimer.h>
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


#define QUEUE_SIZE 8

msg_t msg_queue[QUEUE_SIZE];
mbox_t mbox;
msg_t mbox_queue[QUEUE_SIZE];

void sender(void) {
    msg_t msg;
    msg.type = 0x1234;  // example message type
    while (1) {
        msg.content.value = xtimer_usec_from_nsec(xtimer_now()) % 1000;
        printf("Sending message: %lu\n", msg.content.value);
        mbox_post(&mbox, &msg, 0);
        xtimer_sleep(1);
    }
}

void receiver(void) {
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message: %lu\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, 0, receiver, NULL, "receiver");

    // Let threads run
    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}