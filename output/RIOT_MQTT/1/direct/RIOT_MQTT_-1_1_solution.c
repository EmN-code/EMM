#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/if.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883u
#define MQTT_TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static emcute_sub_t subscriptions[NUMOFSUBS];
static char *topics[NUMOFSUBS];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    (void)topic;
    printf("### received publication for topic '%s' [%i]###\n",
           topic->name, (int)topic->id);
    printf("Data: %.*s\n", (int)len, in);
}

static int publish(const char *topic, const char *data, int qos)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    if (qos == 1) {
        flags = EMCUTE_QOS_1;
    }
    else if (qos == 2) {
        flags = EMCUTE_QOS_2;
    }
    /* step 1: get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", topic);
        return 1;
    }

    printf("Published %s to topic '%s'\n", data, topic);
    return 0;
}

static int connect_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };  // can be AF_INET for IPv4
    char *server_addr = MQTT_BROKER_IP;
    gw.port = MQTT_BROKER_PORT;

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, server_addr) == NULL) {
        puts("error: unable to parse broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", server_addr, (int)gw.port);
        return 1;
    }

    printf("Connected to [%s]:%i\n", server_addr, (int)gw.port);

    return 0;
}

int main(void)
{
    puts("MQTT-SN publish sample application\n");

    /* Step 2: connect to MQTT broker */
    if (connect_broker() != 0) {
        return 1;
    }

    /* Step 3: publish state every INTERVAL seconds */
    while (1) {
        publish(MQTT_TOPIC, "work", 0);
        xtimer_sleep(INTERVAL / US_PER_SEC);
        publish(MQTT_TOPIC, "not work", 0);
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}