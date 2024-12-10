#include <net/sock/util.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <event/callback.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

#define INTERVAL_SEC (1U)
#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

static kernel_pid_t main_thread_pid;
static char main_thread_stack[THREAD_STACKSIZE_MAIN];

Network network;

evtimer_t evtimer;
evtimer_mbox_event_t event;
msg_t msg_queue[8];

void event_handler(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

void *main_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_mbox(&evtimer);

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(INTERVAL_SEC);
            continue;
        } else {
            printf("Network connection successful.\n");
            break;
        }
    }

    event.event.interval.ticks32 = ZTIMER_MSEC_TO_TICKS(1000);
    event.event.callback = event_handler;
    evtimer_add_mbox(&evtimer, &event, &main_thread_pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return NULL;
}

int main(void) {
    main_thread_pid = thread_create(main_thread_stack, sizeof(main_thread_stack),
                                    THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                    main_thread, NULL, "main");
    return 0;
}