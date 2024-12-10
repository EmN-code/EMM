#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <timex.h>
#include <time_units.h>
#include <event/callback.h>
#include <event/thread.h>
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define EVENT_PERIOD   1U  // Event period in seconds
#define EVENT_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char periodic_thread_stack[EVENT_STACKSIZE];

void event_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

void *periodic_thread(void *arg)
{
    (void)arg;
    evtimer_event_t event;
    evtimer_t evtimer;

    evtimer_init(&evtimer, event_handler);

    ztimer_sleep(ZTIMER_SEC, EVENT_PERIOD);

    evtimer_add(&evtimer, &event);
    evtimer_run(&evtimer);

    return NULL;
}

int main(void)
{
    thread_create(periodic_thread_stack, sizeof(periodic_thread_stack), 
                  EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, 
                  periodic_thread, NULL, "periodic_thread");

    return 0;
}