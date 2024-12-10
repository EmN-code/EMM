#include <xtimer.h>
#include <net/mqttsn.h>
#include <net/ipv6/addr.h>
#include <string.h>
#include <stdio.h>

#define MQTT_SN_SERVER_IP "47.102.103.1"
#define MQTT_SN_PORT 1883
#define PUBLISH_INTERVAL 5

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published data on topic '%s'\n", topic->name);
}

int main(void) {
    /* Setup gateway endpoint */
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = MQTT_SN_PORT;
    if (inet_pton(AF_INET, MQTT_SN_SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address\n");
        return -1;
    }

    /* Initialize MQTT-SN */
    emcute_sub_t subscription;
    emcute_topic_t publication;
    publication.name = "device/state";

    /* Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker\n");
        return -1;
    }

    /* Periodically publish data */
    while (1) {
        const char *data = "work";
        if (emcute_pub(&publication, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        }
        on_pub(&publication, (void*)data, strlen(data));
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }

    return 0;
}