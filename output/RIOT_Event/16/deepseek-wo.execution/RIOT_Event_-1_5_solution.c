
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <thread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdatomic.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define MSG_QUEUE_SIZE (16)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_mbox(&evtimer);

    evtimer_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    evtimer_add_periodic(&evtimer, &event.event, 1 * US_PER_SEC);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
