
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <dht_params.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <evtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    printf("Periodic event triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init(&evtimer, event_periodic_callback, NULL);

    // Set up the event to trigger every second
    event.event.offset = 0;
    event.event.interval = PERIODIC_INTERVAL;
    evtimer_add(&evtimer, &event.event);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
