#include <stdio.h>  // Used for printing messages and simulations for assertions.
#include <string.h>  // Required for string manipulations and functions.
#include <stdlib.h>  // Important for memory management (like malloc and free) and general utilities.
#include "xtimer.h" // Provides timer functions to delay or periodically execute tasks in RIOT.
#include "thread.h" // Needed for managing threads in the RIOT operating system.
#include "net/emcute.h" // Allows the application to communicate using the MQTT-SN protocol.

#define EMCUTE_ID          "mqtt-client"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC         "state"
#define MQTT_BROKER_IP     "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[1];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### Got publication for topic '%s' [%i] : %.*s\n", topic->name, topic->id, (int)len, in);
}

static int publish_state(int status) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    char *state_msg = status ? "work" : "not work";

    memset(&t, 0, sizeof(t));
    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
        printf("error: unable to publish data: %s\n", state_msg);
        return 1;
    }

    printf("Published %s on topic %s\n", state_msg, t.name);

    return 0;
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = EMCUTE_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_IP) == NULL) {
        puts("error: unable to parse IPv6 address");
        return NULL;
    }

    if (emcute_con(&gw, true, "", NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    printf("Successfully connected to gateway at [%s]:%i\n", MQTT_BROKER_IP, EMCUTE_PORT);

    while (1) {
        publish_state(1);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN Publishing Demo");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}