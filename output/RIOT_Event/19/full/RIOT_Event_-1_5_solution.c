#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"

#define PRIORITY_MAIN_THREAD (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1000000U) // 1 second in microseconds
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t ev_thread;
evthread_cb_t callback;

void evtimer_handler(void *arg) {
    (void)arg;
    printf("Event triggered at 1 second interval.\n");
}

void *event_thread(void *arg) {
    (void)arg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    evtimer_msg_event_init(&event, evtimer_handler, NULL);

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting event timer demo...");

    ev_thread = thread_create(stack, sizeof(stack), PRIORITY_MAIN_THREAD, 0,
                              event_thread, NULL, "event_proc");

    return 0;
}