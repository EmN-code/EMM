#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <mqtt.h>
#include <net/emcute.h>

#define BROKER_ADDRESS     "47.102.103.1"
#define BROKER_PORT        1883
#define PUBLISH_INTERVAL   (5U * US_PER_SEC)
#define MQTT_TOPIC         "esp32/state"

static emcute_topic_t topic;
static char mqtt_message[64];

static void on_pub(void *arg, int result) {
    if (result != EMCUTE_OK) {
        printf("Publication failed with code %d\n", result);
    } else {
        printf("Published successfully\n");
    }
}

int main(void) {
    printf("Starting MQTT publisher example...\n");

    emcute_topic_t t = {
        .name = MQTT_TOPIC
    };

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };

    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection to MQTT broker failed\n");
        return 1;
    }

    printf("Connected to %s:%d\n", BROKER_ADDRESS, BROKER_PORT);

    while (1) {
        snprintf(mqtt_message, sizeof(mqtt_message), "The state is %s", "work");
        t.id = emcute_reg(&t);

        if (t.id == 0) {
            printf("Failed to register topic\n");
            return 1;
        }

        if (emcute_pub(&t, mqtt_message, strlen(mqtt_message), EMCUTE_QOS_0, (emcute_cb_t) on_pub, NULL) != EMCUTE_OK) {
            printf("Failed to publish\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    emcute_discon();
    return 0;
}