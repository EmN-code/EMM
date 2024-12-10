#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/uart.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define DEFAULT_BROKER_IP   "47.102.103.1"
#define DEFAULT_BROKER_PORT 1883
#define TOPIC               "device/state"
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;
static sock_udp_ep_t gw = {
    .family = AF_INET6,
    .port = DEFAULT_BROKER_PORT
};

void *publish_thread(void *arg) {
    (void)arg;
    char *states[] = {"work", "not work"};
    bool state = true;

    while (1) {
        const char *pub_state = states[state];
        if (emcute_pub(&topic, pub_state, strlen(pub_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish state. Retrying...\n");
        } else {
            printf("Published state: %s\n", pub_state);
        }
        state = !state;
        xtimer_sleep(PUBLISH_INTERVAL/US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    printf("RIOT MQTT-SN Example\n");

    /* convert broker address */
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, DEFAULT_BROKER_IP);

    printf("Connecting to MQTT broker at %s\n", DEFAULT_BROKER_IP);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Unable to register topic\n");
        return 1;
    }

    /* start pub loop */
    char stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "publish");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start publish thread\n");
        return 1;
    }

    emcute_run(DEFAULT_BROKER_PORT, "RIOT_Client");
    return 0;
}