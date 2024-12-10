#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID           "riot-node"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "device/state"
#define PUB_INTERVAL        (5U)

static emcute_topic_t t;
static char stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Published %u bytes
", (unsigned)len);
}

static int mqtt_connect(void)
{
    sock_udp_ep_t gw = {.family = AF_INET6};
    gw.port = EMCUTE_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1") == NULL) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    return 0;
}

static int mqtt_publish(void)
{
    const char *states[] = {"work", "not work"};
    unsigned long count = 0;
    while (1) {
        const char *state = states[count % 2];
        printf("Publishing state: %s\n", state);

        ssize_t result = emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0);
        if (result < 0) {
            printf("Error: unable to publish data (%d)\n", result);
            break;
        }
        xtimer_sleep(PUB_INTERVAL);
        count++;
    }
    return 0;
}

int main(void)
{
    puts("MQTT-SN example");

    emcute_topic_t t = {
        .name = TOPIC_NAME
    };

    /* initialize package and connect to broker */
    if (emcute_init(stack, sizeof(stack), EMCUTE_PRIO) != EMCUTE_OK) {
        puts("Error: unable to initialize emcute");
        return 1;
    }

    if (mqtt_connect() != 0) {
        return 1;
    }

    /* subscribe to a topic */
    if (emcute_sub(&t, 0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    mqtt_publish();

    /* shouldn't reach here */
    return 0;
}