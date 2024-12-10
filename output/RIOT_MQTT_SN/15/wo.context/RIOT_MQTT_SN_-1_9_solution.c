#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emcute.h>
#include <net/af.h>
#include <net/conn/udp.h>
#include <core.h>
#include <timex.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char *topic_name = "device/state";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i] : %.*s\n",
           topic->name, topic->id, (int)len, in);
}

static int connect_gateway(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    
    if (inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6) == 0) {
        printf("Failed to encode IPv6 address\n");
        return 1;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to gateway at [%s]:%i\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            break;
        }
        else {
            printf("Connection attempt failed: %s\n", emcute_type_str(res));
            xtimer_sleep(1);
        }
    }
    return 0;
}

static int publish_state(const char *state) {
    while (1) {
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Published %s on topic %s\n", state, topic_name);
            break;
        }
        else {
            printf("Publish attempt failed: %s\n", emcute_type_str(res));
            xtimer_sleep(1);
        }
    }
    return 0;
}

int main(void) {
    /* connect to MQTT-SN broker */
    if (connect_gateway() != 0) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    /* Register topic */
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
    }

    /* disconnect (unreachable in this example, but good practice) */
    emcute_discon();

    return 0;
}

