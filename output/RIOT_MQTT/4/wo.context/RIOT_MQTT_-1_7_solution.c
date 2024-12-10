#include <xtimer.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/emcute/mqtt-sn.h>
#include <MQTTClient.h>

#define EMCUTE_ID          "riot-client"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define CONNECTION_RETRY   5
#define PUBLISH_INTERVAL   (5 * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static const sock_udp_ep_t broker_endpoint = {
    .family = AF_INET6,
    .port = 1883,
    .addr = { .ipv6 = { 0x47, 0x102, 0x103, 0x1 } }  // Placeholder for IPv6 address representation
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication on topic: %s [%.*s]\n", topic->name, (int)len, (char *)data);
}

static int publish(const char *topic_name, const char *message) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    
    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    
    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }
    printf("Published [%s] on topic '%s'\n", message, topic_name);
    return 0;
}

int main(void) {
    msg_init_queue(queue, 8);

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    printf("Connecting to Gateway at [%u.%u.%u.%u]:%i\n", broker_endpoint.addr.ipv4[0], broker_endpoint.addr.ipv4[1],
           broker_endpoint.addr.ipv4[2], broker_endpoint.addr.ipv4[3], broker_endpoint.port);

    if (emcute_con((sock_udp_ep_t *)&broker_endpoint, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Unable to connect to broker");
        return 1;
    }

    while (1) {
        publish("state", "work");  // Periodically publish "work" state
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);  // Sleep for the defined interval
    }

    emcute_discon();

    return 0;
}