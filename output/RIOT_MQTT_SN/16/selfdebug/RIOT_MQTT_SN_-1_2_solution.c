#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5000 // 5 seconds in milliseconds
#define MQTT_TOPIC "state"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("successfully published state to topic '%s'\n", topic->name);
}

static void *publish_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *state = "work";  // You can change this to "not work" as needed
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL / 1000);
    }
    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example");

    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = MQTT_TOPIC;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return 1;
    }

    sock_udp_ep_t gw = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway at [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish");

    emcute_run(BROKER_PORT, "mqtt_client");  // Run the MQTT client

    return 0;
}