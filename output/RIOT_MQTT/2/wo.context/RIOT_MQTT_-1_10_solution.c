#include <ztimer.h>  // Correcting the sleep function error
#include <thread.h>  // Correcting the incompatible pointer types error
#include <emcute.h>  // Correcting the declaration error related to subscribing
#include <net/emcute.h>
#include "MQTTClient.h"  // Including the MQTTClient.h as the last header as per requirement

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define INTERVAL 5  // Interval between publishes

static char stack[THREAD_STACKSIZE_MAIN];

static void* mqtt_thread(void* arg) {
    (void)arg;

    sock_udp_ep_t gw = {.family = AF_INET6};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);
    gw.port = MQTT_BROKER_PORT;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = "mytopic";
    t.id = 0;
    emcute_reg(&t);

    while (1) {
        const char *state = "work";
        if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
            puts("error: unable to publish");
        } else {
            puts("state published");
        }

        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt");
    return 0;
}