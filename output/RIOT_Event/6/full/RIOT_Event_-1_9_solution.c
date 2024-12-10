#include "esp_system.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "stdio.h"
#include "stdlib.h"
#include "event/thread.h"
#include "thread.h"
#include "msg.h"
#include "msg_init_queue"
#include "ztimer.h"
#include "timex.h"
#include "event/periodic.h"
#include "event_periodic.h"

#define INTERVAL (1U * US_PER_SEC)

static void my_event_periodic_callback(void *arg) {
    (void)arg;
    /* Callback function logic here */
    printf("Event periodic callback triggered.\n");
}

int main(void) {
    /* Initialize message queue */
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    /* Initialize periodic event */
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, EVENT_PRIO_HIGHEST, my_event_periodic_callback, NULL);

    /* Start the periodic event with defined interval */
    event_periodic_start(&periodic_event, INTERVAL);

    /* Run the event loop */
    event_run();
    return 0;
}