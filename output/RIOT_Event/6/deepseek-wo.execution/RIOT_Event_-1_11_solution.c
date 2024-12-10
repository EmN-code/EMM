
#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <mutex.h>
#include <stdbool.h>
#include <stddef.h>
#include <timex.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <time_units.h>
#include <xtimer.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_t *event = (evtimer_msg_event_t *)msg.content.ptr;
            event->event.callback(event->event.arg);
        }
    }

    return 0;
}
