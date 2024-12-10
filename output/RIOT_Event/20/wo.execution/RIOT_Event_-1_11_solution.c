#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdio.h"

static evtimer_t my_event_timer;
static evtimer_msg_event_t my_event;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer callback executed!\n");
    /* Re-add the event to create a periodic effect */
    my_event.event.offset += 1000000;  // Offset in microseconds for 1 second
    evtimer_add_msg(&my_event_timer, &my_event, thread_getpid());
}

int main(void)
{
    puts("RIOT Event Timer Example");

    /* Initialize the event timer for message handling */
    evtimer_init_msg(&my_event_timer);

    /* Setting up the event properties */
    my_event.callback = timer_callback;
    my_event.event.offset = 1000000;  // Period of 1 second in microseconds

    /* Add the event to the timer */
    evtimer_add_msg(&my_event_timer, &my_event, thread_getpid());

    /* Run the event loop */
    while (1) {
        evtimer_mbox_event_t mbox_event;
        evtimer_add_mbox(&my_event_timer, &mbox_event, (mbox_t *) NULL);
        thread_yield();
    }

    return 0;
}