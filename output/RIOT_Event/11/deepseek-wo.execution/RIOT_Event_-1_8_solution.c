
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    (void)arg;
}

int main(void)
{
    xtimer_t timer;
    msg_t msg_array[16];
    msg_t msg;
    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;

    msg_init_queue(msg_array, 16);

    evtimer_init_mbox(&evtimer);

    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    xtimer_set(&timer, PERIODIC_INTERVAL);

    while (1) {
        msg_receive(&msg);
        xtimer_spin(&timer);
    }

    return 0;
}
