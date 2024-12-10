#include <ztimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_TOPIC      "device/status"

static char stack[THREAD_STACKSIZE_MAIN];
static char recv_stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[1];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, sizeof(stack));
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("Received Publication on topic %s [%i]: %s\n", topic->name, (int)topic->id, in);
}

int main(void) {
    // Initialize the EMQTT-SN client (emcute)
    memset(subscriptions, 0, sizeof(subscriptions));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    // Set up the remote endpoint (MQTT-SN broker)
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Connect to the gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, EMCUTE_PORT);
        return 1;
    }
    printf("Connected to MQTT-SN broker\n");

    while (1) {
        // Publish state periodically
        const char *state = "work";
        emcute_reg(&topic);
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);

        printf("Published: %s
", state);
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}