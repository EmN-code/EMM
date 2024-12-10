#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <arpa/inet.h>
#include <stdatomic.h>

#define INTERVAL_USEC (1000000U) // 1 second

static evtimer_t evtimer;
static evtimer_mbox_event_t mbox_event;
static mbox_t mbox;

void led_toggle_callback(evtimer_event_t *event){
    // Mock implementation for toggling LED
    printf("LED toggled\n");
}

int main(void){
    msg_init_queue(msg_q, sizeof(msg_q) / sizeof(msg_t));

    evtimer_init_mbox(&evtimer);
    mbox_init(&mbox, mbox_buf, sizeof(mbox_buf));
    evtimer_set_callback(&evtimer, led_toggle_callback);
    
    evtimer_mbox_event_init(&mbox_event, &mbox);
    evtimer_add(&evtimer, &mbox_event.event, INTERVAL_USEC, 1);
    
    while(1) {
        evtimer_tick(&evtimer);
        usleep(INTERVAL_USEC);
    }

    return 0;
}