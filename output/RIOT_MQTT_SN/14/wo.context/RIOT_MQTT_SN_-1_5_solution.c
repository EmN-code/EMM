#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT         1883
#define MQTT_SN_ADDRESS     "47.102.103.1"
#define PUB_INTERVAL        (5U * US_PER_SEC) // publish interval in microseconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *msg = (char *)data;
    printf("Received publication for topic '%s' [%i]: %s\n",
           topic->name, (int)topic->id, msg);
}

int main(void)
{
    sock_udp_ep_t gw;
    gw.port = EMCUTE_PORT;

    if (inet_pton(AF_INET, MQTT_SN_ADDRESS, &gw.family) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    // Start emcute thread
    emcute_run(THREAD_PRIORITY_MAIN - 1, stack, sizeof(stack), "emcute");

    // Connect to MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published '%s' to topic '%s'\n", state, topic.name);
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}