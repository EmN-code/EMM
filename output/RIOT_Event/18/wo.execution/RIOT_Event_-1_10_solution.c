#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "msg.h"
#include "mbox.h"
#include "event/callback.h"
#include "event/thread.h"
#include "event/timeout.h"
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void timer_callback(void *arg)
{
    (void)arg;
    printf("Event triggered\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t mbox_event;
    mbox_t mbox;

    msg_t queue[8];
    mbox_init(&mbox, queue, 8);

    mbox_event.event.absolute = ztimer_now(ZTIMER_MSEC) + 1000;
    mbox_event.event.period = 1000;
    mbox_event.event.callback = timer_callback;

    evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}