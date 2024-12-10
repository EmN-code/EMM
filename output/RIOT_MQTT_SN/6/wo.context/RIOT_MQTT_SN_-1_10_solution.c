#include "net/emcute.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define EMCUTE_ID           "station42"

static emcute_sub_t subscriptions[CONFIG_EMCUTE_MAX_SUBSCRIPTIONS];
static char stack[THREAD_STACKSIZE_MAIN];

static emcute_topic_t topic;
static char pub_data[64];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;

    puts("Received something!");
}

static int mqtt_connect(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(gw.family, MQTT_BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    return 0;
}

static int mqtt_publish(void)
{
    sprintf(pub_data, "station is %s", (rand() % 2) ? "work" : "not work");
    topic.name = "station_state";
    topic.id = 0; // Let the broker assign the ID
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic, return code %d\n", res);
        return 1;
    }

    if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to publish data\n");
        return 1;
    }

    return 0;
}

int main(void)
{
    puts("MQTT-SN Client example\n");

    memset(subscriptions, 0, sizeof(subscriptions));
    emcute_init(stack, sizeof(stack), EMCUTE_PRIO);

    if (mqtt_connect() != 0) {
        return 1;
    }

    emcute_sub(&subscriptions[0], EMCUTE_QOS_0, on_pub);

    while (1) {
        if (mqtt_publish() != 0) {
            return 1;
        }
        xtimer_sleep(5);
    }

    return 0;
}
