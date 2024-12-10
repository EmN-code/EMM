#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mqtt.h>
#include <net/emcute.h>

define EMCUTE_ID          "mqtt-client"
define EMCUTE_PORT        (1883U)
define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define TOPIC       "esp32/status"
#define DELAY (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *_mqtt_thread(void *arg) {
    (void)arg;

    /* Connect to MQTT broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, "47.102.103.1");
    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        printf("error: unable to connect to the gateway\n");
        return NULL;
    }

    printf("Successfully connected to broker at ");
    fmt_ipv6_addr(&addr, addr.str, sizeof(addr.str));
    printf("%s\n", addr.str);

    /* Set up topic */
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    printf("Registered topic: %s\n", TOPIC);

    char *message = "work";

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish message\n");
        } else {
            printf("Published: %s\n", message);
        }

        xtimer_sleep(DELAY);
    }

    return NULL;
}

int main(void) {
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, "emcute_thread");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  _mqtt_thread, NULL, "mqtt\_thread");

    return 0;
}