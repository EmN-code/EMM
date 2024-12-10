#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           "esp32node"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC         "device/state"
#define BROKER_IP           "47.102.103.1"

static emcute_topic_t state_topic;
static char mqtt_topic[32];
static emcute_sub_t subscriptions[1];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i] : %s\n",
           topic->name, (int)topic->id, in);
}

static int publish(const char *topic, const char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    (void)qos;

    /* step 1: get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("[mqtt-sn] error: unable to obtain topic ID\n");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("[mqtt-sn] error: unable to publish data\n");
        return 1;
    }

    printf("[mqtt-sn] Published %s on topic %s\n", data, topic);

    return 0;
}

static int start_mqtt(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    char *topic = STATE_TOPIC;

    /* parse address of the MQTT-SN broker */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        printf("[mqtt-sn] error: unable to parse broker address\n");
        return 1;
    }

    /* initialize the emcute client */
    if (emcute_sock(&gw, EMCUTE_PORT, EMCUTE_PRIO) != EMCUTE_OK) {
        puts("error: unable to initialize emcute\n");
        return 1;
    }

    /* subscribe to topic */
    memset(subscriptions, 0, (1 * sizeof(emcute_sub_t)));
    subscriptions[0].cb = on_pub;
    strncpy(mqtt_topic, topic, sizeof(mqtt_topic));
    subscriptions[0].topic.name = mqtt_topic;

    if (emcute_sub(&subscriptions[0], 0) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", topic);
        return 1;
    }

    puts("Successfully connected to MQTT-SN broker\n");

    return 0;
}

int main(void) {
    printf("MQTT-SN example application\n");

    if (start_mqtt() != 0) {
        puts("Failed to initialize and start MQTT\n");
        return 0;
    }

    while (1) {
        publish(STATE_TOPIC, "work", 0);
        xtimer_sleep(5);
        publish(STATE_TOPIC, "not work", 0);
        xtimer_sleep(5);
    }

    return 0;
}