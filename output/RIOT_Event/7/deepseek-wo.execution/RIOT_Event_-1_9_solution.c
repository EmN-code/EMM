
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    evtimer_add_msg(&evtimer, (evtimer_event_t *)arg);
}

int main(void)
{
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event for the periodic callback
    event.event.handler = (event_handler_t)periodic_callback;
    event.event.arg = &event;
    event.interval = TIMER_INTERVAL;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event);

    // Start the periodic event
    evtimer_start(&evtimer, &event);

    // Yield to allow other threads to run
    thread_yield();

    return 0;
}
