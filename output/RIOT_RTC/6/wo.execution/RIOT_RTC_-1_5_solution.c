#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "msg.h"
#include "unistd.h"

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    struct timeval now;
    now.tv_sec = mktime(&(struct tm){.tm_year=2024-1900, .tm_mon=5, .tm_mday=1});
    now.tv_usec = 0;

    struct timeval interval;
    interval.tv_sec = 5;
    interval.tv_usec = 0;

    evtimer_t evtimer;
    evtimer_msg_event_t event;
    evtimer_msg_init(&evtimer);
    evtimer_msg_set(&evtimer, &event, &msg_queue[0], &interval);

    while (1) {
        msg_receive(&msg_queue[0]);
        alarm_callback(NULL);
    }

    return 0;
}