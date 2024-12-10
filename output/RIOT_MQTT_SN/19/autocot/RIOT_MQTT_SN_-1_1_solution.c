#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "ztimer.h"

#define EMCUTE_ID          "esp32_device"
#define BROKER_IP          "47.102.103.1"
#define BROKER_PORT        1883
#define PUBLISH_INTERVAL   (5U)
#define TOPIC_NAME         "device/state"

static emcute_topic_t topic;
static char mqtt_payload[64];

// MQTT-SN client setup and connection to the broker
static int mqtt_sn_init_and_connect(void) {
    emcute_sub_t emcute_sub;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

    /* parse the broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        printf("Error: unable to parse IPv6 address\n");
        return 1;
    }

    /* initialize emcute */
    emcute_init();

    /* connect UDP endpoint */
    if (emcute_con(&gw, true, EMCUTE_ID, NULL, &emcute_sub, 0) != 0) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    /* set up the topic */
    topic.name = TOPIC_NAME;

    return 0;
}

// Publish state to the broker
static void publish_state(void) {
    static bool work_state = false;
    /* toggle state between "work" and "not work" */
    work_state = !work_state;
    snprintf(mqtt_payload, sizeof(mqtt_payload), "Device is %s", work_state ? "working" : "not working");
    
    if (emcute_pub(&topic, mqtt_payload, strlen(mqtt_payload), EMCUTE_QOS_0) != 0) {
        printf("Error: unable to publish\n");
    }
}

int main(void) {
    if (mqtt_sn_init_and_connect() != 0) {
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }

    return 0;
}