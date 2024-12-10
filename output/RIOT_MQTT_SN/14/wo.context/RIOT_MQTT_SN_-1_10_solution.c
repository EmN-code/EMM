#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUBLICATION_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("Received publication on topic "%s": %.*s\n",
           topic->name, (int)len, in);
}

static int _publish(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    emcute_topic_t topic;
    topic.name = "esp32/state";
    topic.id = 0;  // Set the topic ID to 0

    return emcute_pub(&topic, state, strlen(state), flags);
}

int main(void) {
    int res;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    msg_init_queue(queue, ARRAY_SIZE(queue));
    memset(stack, 0, sizeof(stack));

    printf("Connecting to the broker at %s...\n", MQTT_BROKER_IP);
    if (inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6) != 1) {
        puts("Failed to parse broker address\n");
        return 1;
    }

    if ((res = emcute_con(&gw, true, NULL, NULL, 0, 0)) != EMCUTE_OK) {
        printf("Unable to connect to [%s]:%d (%d)\n", MQTT_BROKER_IP, EMCUTE_PORT, res);
        return 1;
    }
    puts("Connected\n");

    while (1) {
        _publish("work");
        xtimer_sleep(PUBLICATION_INTERVAL);
        _publish("not work");
        xtimer_sleep(PUBLICATION_INTERVAL);
    }

    return 0;
}