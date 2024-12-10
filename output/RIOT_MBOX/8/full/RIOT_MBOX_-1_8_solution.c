#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <timex.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <dht.h>
#include <thread.h>
#include <thread_create.h>
#include <kernel_defines.h>
#include <time_units.h>

#define INTERVAL (1000000U) // Interval in microseconds

static msg_t queue[8];
static mbox_t mbox;
static char stack[THREAD_STACKSIZE_MAIN];

void *message_handler(void *arg) {
    (void)arg;
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message type %u with content %s\n", msg.type, (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, queue, sizeof(queue)/sizeof(msg_t));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, message_handler, NULL, "msg_handler");

    char *msg_content = "Hello, RIOT!";

    while (1) {
        xtimer_usleep(INTERVAL);

        msg_t msg;
        msg.type = 1;
        msg.content.ptr = msg_content;

        if (mbox_put(&mbox, &msg, 0) < 0) {
            printf("Error: failed to put message into mailbox\n");
        }
    }

    return 0;
}