#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <msg.h>
#include <xtimer.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <timex.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <event/thread.h>
#include <mutex.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define INTERVAL_SEC (1U)

static evtimer_t evtimer;
static evtimer_event_t event;

void event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    /* Initialize the message-based event timer */
    evtimer_init_msg(&evtimer);

    /* Setup the event with the callback */
    event.callback = event_callback;
    event.period = INTERVAL_SEC * US_PER_SEC;

    /* Add event to the timer */
    evtimer_add_event(&evtimer, &event);

    /* Run the evtimer (usually it's managed as part of the main loop) */
    while (1) {
        evtimer_run(&evtimer);
        thread_yield();
    }

    return 0;
}