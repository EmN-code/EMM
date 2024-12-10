#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_ID           "riot-node"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC          "device/status"
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int publish_state(void) {
    char *state = "work";
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("Published %s to topic %s.\n", state, MQTT_TOPIC);
    return 0;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", MQTT_BROKER_ADDR, (int)gw.port);
        return 1;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic %s\n", topic.name);
        return 1;
    }

    printf("Connected to MQTT broker at [%s]:%d\n", MQTT_BROKER_ADDR, (int)gw.port);
    return 0;
}

int main(void) {
    puts("Starting MQTT example\n");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (connect_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}