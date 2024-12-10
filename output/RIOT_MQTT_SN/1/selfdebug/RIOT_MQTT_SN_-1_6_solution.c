#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <emcute.h>

#define MSG_TYPE "work or not work"

static emcute_topic_t _topic;

void on_publish(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Published message on topic %s: %s\n", topic->name, (char *)data);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    emcute_sub_t subscription = {
        .topic.name = "state",
        .cb = on_publish,
        .arg = NULL
    };

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker.\n");
        return 1;
    }

    _topic.name = "state";
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        return 1;
    }

    if (emcute_sub(&subscription, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to subscribe to topic.\n");
        return 1;
    }

    while (1) {
        if (emcute_pub(&_topic, MSG_TYPE, strlen(MSG_TYPE), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message.\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}