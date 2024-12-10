#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <MQTTClient.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <timex.h>
#include <xtimer.h>
#include <time_units.h>
#include <mbox.h>
#include <evtimer_msg.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5
#define EVENT_INTERVAL (1000000LU)

static mbox_t my_mbox;
static msg_t msg_queue[8];
static evtimer_t evtimer;
static evtimer_mbox_event_t event;

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&my_mbox, &msg, 1);
        printf("Message received!\n");
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    mbox_init(&my_mbox, msg_queue, 8);
    evtimer_init_mbox(&evtimer);

    kernel_pid_t thread_pid = thread_create(
        NULL, THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN, 0, thread_handler,
        NULL, "event_handler"
    );

    evtimer_add_mbox(&evtimer, &event, &my_mbox);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, EVENT_INTERVAL);
        event.event.offset = EVENT_INTERVAL;
        mbox_post(&my_mbox, NULL);
    }

    return 0;
}