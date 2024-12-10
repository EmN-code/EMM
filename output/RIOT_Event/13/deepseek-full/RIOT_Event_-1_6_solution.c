
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE 8
#define PERIODIC_INTERVAL (1 * US_PER_SEC)

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static mbox_t _mbox;
static evtimer_t _evtimer;

static void _periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    mbox_init(&_mbox, _msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_mbox(&_evtimer);

    while (1) {
        evtimer_add_msg(&_evtimer, PERIODIC_INTERVAL, _periodic_callback, NULL);
        msg_t msg;
        mbox_get(&_mbox, &msg, 1);
        if (msg.type == EVTIMER_MSG_TICK) {
            _periodic_callback(msg.content.ptr);
        }
    }

    return 0;
}
