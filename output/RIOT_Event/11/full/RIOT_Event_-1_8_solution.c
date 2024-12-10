#include "xtimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "mbox.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "timex.h"
#include "esp_system.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#define INTERVAL (1U * US_PER_SEC)

evtimer_t event_timer;

static void my_callback(void *arg) {
    printf("Event Triggered!\n");
    (void)arg; // to avoid unused variable warning
}

int main(void) {
    evtimer_init_msg(&event_timer);
    evtimer_tick(msg); // Assuming evtimer_tick is available to trigger the event

    evtimer_event_t event;
    event.callback = my_callback;
    event.arg = NULL;

    evtimer_add(&event_timer, &event);

    while (true) {
        xtimer_sleep(INTERVAL);
    }
    return 0;
}