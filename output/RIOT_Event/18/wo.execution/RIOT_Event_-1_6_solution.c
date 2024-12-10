#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <event/timeout.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define INTERVAL (1U * US_PER_SEC)
#define MAIN_QUEUE_SIZE (8)

static msg_t _msg_queue[MAIN_QUEUE_SIZE];

void callback(void *arg)
{
    (void)arg;
    printf("Callback triggered every second.\n");
}

int main(void)
{
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        printf("Waiting for timer callback...\n");
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        callback(NULL);
    }

    return 0;
}