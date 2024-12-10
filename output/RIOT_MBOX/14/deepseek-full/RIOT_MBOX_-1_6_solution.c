
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <dht_params.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <thread.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <mma8x5x_params.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void event_handler(void *arg)
{
    (void)arg;
    printf("Event received!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_handler(NULL);
    }

    return 0;
}
