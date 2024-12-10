#include <xtimer.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#define INTERVAL (1U * US_PER_SEC)

static void _periodic_handler(evtimer_t *event, void *arg) {
    (void) event;
    (void) arg;
    puts("Periodic Event Triggered!");
}

int main(void) {
    evtimer_msg_event_t evt_msg;
    evtimer_t evtimer;
    msg_t message_queue[8];
    msg_init_queue(message_queue, 8);

    evtimer_set_msg(&evtimer, event_msg);
    evtimer_add_msg(&evtimer, &evt_msg, XTIMER_MSG, THREAD_ID);
    evtimer_init_mbox(&evtimer);

    while (1) {
        evtimer_periodic_event_start(&evtimer, INTERVAL, _periodic_handler, NULL);
        msg_receive(&msg);
    }
    return 0;
}