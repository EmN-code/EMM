#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define MQTT_MSG "work"
#define PUBLISH_INTERVAL 5U

static emcute_topic_t device_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = MQTT_BROKER_PORT
    };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP) == NULL) {
        printf("Error: unable to parse broker address\n");
        return -1;
    }

    while (1) {
        int rc = emcute_con(&gw, true, "device_id", NULL, 0);
        if (rc != EMCUTE_OK) {
            printf("Error: could not connect to broker [%d]\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to broker\n");
        return 0;
    }
}

static int publish_state(void) {
    device_topic.name = MQTT_TOPIC;
    device_topic.id = 0;

    while (1) {
        int rc = emcute_pub(&device_topic, MQTT_MSG, strlen(MQTT_MSG), EMCUTE_QOS_1);
        if (rc != EMCUTE_OK) {
            printf("Error: unable to publish [%d]\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("Published: %s\n", MQTT_MSG);
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}

int main(void) {
    printf("Starting MQTT-SN example\n");

    /* Initialize the emcute instance */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    if (connect_broker() != 0) {
        printf("Unable to connect to the broker\n");
        return -1;
    }

    /* Start publishing state */
    publish_state();

    return 0;
}