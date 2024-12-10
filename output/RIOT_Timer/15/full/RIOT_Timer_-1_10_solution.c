#include "net/sock/util.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "esp_system.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "time_units.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "stdatomic.h"
#include "stdbool.h"

#define INTERVAL (1U) // Timer interval in seconds

static evtimer_t evtimer;
static evtimer_mbox_event_t event;
static mbox_t mbox;

void event_handler(evtimer_event_t *event)
{
    // Simulate toggling a GPIO controlled LED
    printf("LED Toggled\n");
}

int main(void)
{
    // Initialize mbox
    char mbox_buf[8];
    mbox_init(&mbox, mbox_buf, sizeof(mbox_buf));

    // Initialize event timer for mbox events
    evtimer_init_mbox(&evtimer);

    // Initialize the event
    evtimer_mbox_event_init(&event, event_handler);

    // Set and start the timer
    evtimer_add(&evtimer, (evtimer_event_t *)&event, INTERVAL * US_PER_SEC);

    // Main loop
    while (1) {
        evtimer_tick();
        usleep(INTERVAL * US_PER_SEC);
    }

    return 0;
}