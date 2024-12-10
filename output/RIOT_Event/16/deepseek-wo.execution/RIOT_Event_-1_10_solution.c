
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <xtimer.h>
#include <thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <mutex.h>
#include <dht.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void event_periodic_callback(void *arg)
{
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.callback = event_periodic_callback;
    event.event.arg = NULL;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    event.event.interval = interval;

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            event_periodic_callback(msg.content.ptr);
        }
    }

    return 0;
}
