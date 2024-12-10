#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "string.h"
#include "stdatomic.h"
#include "setup_timer.h"
#include "toggle_led_mock"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "kernel_defines.h"

#define TIMER_INTERVAL (1U * 1000U * 1000U) // 1 second

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

void timer_callback(void *arg) {
    (void)arg;
    // Here we would toggle the LED; you can replace 'toggle_led_mock()' with the actual function.
    toggle_led_mock();
}

int main(void) {
    puts("Starting LED timer example...");

    evtimer_init(&evtimer, ztimer_now(ZTIMER_SEC), timer_callback, NULL);
    evtimer_set(&evtimer, &evtimer_event, TIMER_INTERVAL);

    // Loop indefinitely to handle the timer event
    while (true) {
        evtimer_dispatch(&evtimer);
    }
    return 0;
}
