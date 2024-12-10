#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "string.h"
#include "periph/gpio.h"
#include "net/emcute.h"

#define EMCUTE_ID "esp32"
#define EMCUTE_PORT (1883U)
#define EMCUTE_QOS (0)
#define TOPIC_NAME "device/state"
#define BROKER_ADDRESS "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[1];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_sub_t sub;
    sub.topic.name = TOPIC_NAME;
    sub.cb = on_pub;
    subscriptions[0] = sub;

    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(gw.family, BROKER_ADDRESS, &gw.addr.ipv4);

    /* Initialize the emcute network */
    if (emcute_sock(&gw) != EMCUTE_OK) {
        puts("Could not initialize MQTT-SN network");
        return NULL;
    }

    /* Connect to MQTT-SN broker */
    if (emcute_con(EMCUTE_ID, true, NULL, 0) != EMCUTE_OK) {
        puts("MQTT-SN connection failed");
        return NULL;
    }

    while (1) {
        const char *state = "work";
        emcute_topic_t t;
        t.name = TOPIC_NAME;
        emcute_reg(&t);

        emcute_pub(&t, state, strlen(state), EMCUTE_QOS);
        puts("State published to broker");

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("Starting the MQTT-SN publisher...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");
    
    xtimer_sleep(1);  // Allow background operations

    return 0;
}