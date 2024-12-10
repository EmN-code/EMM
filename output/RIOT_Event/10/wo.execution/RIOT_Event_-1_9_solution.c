#include <xtimer.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <mutex.h>
#include <timex.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define INTERVAL_SEC 1

void callback(void *arg) {
    printf("Timer callback function triggered every second.\n");
}

int main(void) {
    printf("RIOT event periodic example\n");

    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;
    evtimer_init_msg(&evtimer);

    event.event.offset = INTERVAL_SEC * US_PER_SEC;
    event.event.period = INTERVAL_SEC * US_PER_SEC;

    kernel_pid_t main_pid = thread_getpid();
    evtimer_add_msg(&evtimer, &event, main_pid);

    while (1) {
        pause();
    }

    return 0;
}