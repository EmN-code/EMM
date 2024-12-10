
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <mutex.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/gcoap.h>

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;
static evtimer_msg_event_t event;

void event_periodic_callback(void *arg)
{
    printf("Periodic event triggered!\n");
    evtimer_add_msg(&evtimer, &event, thread_getpid());
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    event.event.callback = event_periodic_callback;
    event.event.arg = NULL;
    event.event.offset = 0;
    event.event.interval = 1 * US_PER_SEC;

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
