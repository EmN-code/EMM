#include <xtimer.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <mbox.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)

static void *event_periodic_callback(void *arg) {
    (void)arg;
    while (1) {
        printf("Periodic event triggered every second.\n");
        sleep(1);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN, 0,
                  event_periodic_callback, NULL, "event_thread");

    return 0;
}