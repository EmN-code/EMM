#include "net/sock/util.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"

#define LED_PIN 2  // Example pin for ESP32
#define TIMER_INTERVAL_SEC 1  // 1 second interval

void led_on(void) {
    // Custom function to turn on LED, implementation will depend
    // on the actual way we have to control the pin on RIOT OS.
    printf("LED turned on.\n");
}

void evtimer_handler(void *arg) {
    (void)arg;  // Unused
    led_on();
}

int main(void) {
    // Setup a recurring timer using evtimer and ztimer
    evtimer_t evtimer;
    evtimer_t *evtimer_ptr = &evtimer;
    msg_t msg_queue[16];  // Message queue for event handling
    msg_init_queue(msg_queue, 16);

    while (1) {
        int rc = evtimer_msg_init(evqueue_ptr, EVT_MSG_TYPE_PERIODIC, msg_queue, 16);
        if (rc != 0) {
            printf("evtimer_msg_init failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        else {
            printf("evtimer_msg_init successfully\n");
            break;
        }
    }

    ztimer_t periodic_timer;
    ztimer_set_wakeup_periodic(ZTIMER_SEC, &periodic_timer, TIMER_INTERVAL_SEC, evtimer_handler, evtimer_ptr);
    printf("Starting periodic timer with interval: %d seconds\n", TIMER_INTERVAL_SEC);

    // Main loop
    while (1) {
        // Could be some idle work here
        pause();  // Let the system sleep until the next interrupt
    }

    return 0;
}