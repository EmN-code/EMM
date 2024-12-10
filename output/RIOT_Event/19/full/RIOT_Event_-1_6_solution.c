#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <thread.h>
#include <timex.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN - 64)
#define LED_THREAD_PRIORITY      (THREAD_PRIORITY_MAIN + 1)

evtimer_t evtimer;
static evtimer_msg_event_t event;

void callback(void *arg) {
    (void)arg;
    printf("Callback triggered every second\n");
    evtimer_add_msg(&evtimer, &event, thread_getpid());
}

int main(void) {
    evtimer_init_mbox(&evtimer);
    evtimer_msg_event_init(&event);
    event.event.time = 1000000U; // 1 second in microseconds
    event.event.callback = callback;
    evtimer_add_msg(&evtimer, &event, thread_getpid());
    while (1) {
        sleep(1);
    }
    return 0;
}