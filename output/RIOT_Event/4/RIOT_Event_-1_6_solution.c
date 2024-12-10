
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "stddef.h"
#include "mma8x5x.h"
#include "ztimer.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "timex.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mbox.h"
#include "string.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "msg.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "netinet/in.h"
#include "periph/rtc.h"

#define INTERVAL (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t queue;

    event_queue_init(&queue);

    event_periodic_callback_create(&event, ZTIMER_USEC, INTERVAL, &queue, periodic_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_handle(&msg);
    }

    return 0;
}
