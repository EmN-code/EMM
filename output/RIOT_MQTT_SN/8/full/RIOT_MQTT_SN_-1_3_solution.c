#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "event/thread.h"

// Definitions for connecting to the MQTT-SN broker
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define TOPIC_NAME          "device/state"
#define QOS                 0
#define PUB_INTERVAL        5

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[CONFIG_EMCUTE_MAX_SUBS] = {0};
static char pub_data[64];

static void *mqtt_thread(void *arg) {
    (void) arg;

    sock_udp_ep_t broker;
    broker.family = AF_INET6;
    broker.port = MQTT_BROKER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&broker.addr.ipv6, MQTT_BROKER_IP);

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Register the topic
    while (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[ERROR] unable to obtain topic ID\n");
        thread_sleep(1);
    }

    // Connect to broker
    unsigned flags = EMCUTE_QOS_0;
    while (emcute_con(&broker, true, NULL, NULL, 0, flags) != EMCUTE_OK) {
        printf("[ERROR] unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        thread_sleep(1);
    }

    // Publish data every 5 seconds
    while (1) {
        snprintf(pub_data, sizeof(pub_data), "{" "state": " "work" }";
        if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[ERROR] unable to publish data\n");
        }
        thread_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_thread, NULL, "mqtt");
    return 0;
}
