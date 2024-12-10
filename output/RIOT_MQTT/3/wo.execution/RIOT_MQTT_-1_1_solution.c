#include "xtimer.h"
#include "net/emcute.h"
#include "thread.h"
#include "stdio.h"

#define EMCUTE_ID           (1)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883)
#define EMCUTE_ADDRESS      "47.102.103.1"
#define MQTT_TOPIC          "device/state"
#define MQTT_MESSAGE        "work"
#define PUBLISH_INTERVAL    (5U * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

/* Function to print state and publish it to MQTT broker */
static void *mqtt_publisher(void *arg) {
    (void)arg;

    msg_t msg;

    while (1) {
        xtimer_sleep(PUBLISH_INTERVAL / 1000000U);

        printf("Publishing MQTT message: %s\n", MQTT_MESSAGE);

        /* Publish the message to the broker */
        mqtt_publish(&emcute, &topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), EMCUTE_QOS_0);
    }

    return NULL;
}

int main(void) {
    /* Initialize the emcute device */
    emcute_topic_t t;
    t.name = MQTT_TOPIC;
    topic = t;

    /* Connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, EMCUTE_ADDRESS, &gw.addr.ipv6);
    emcute_con(&emcute, &gw, true, NULL, NULL, 0, 0);

    /* Subscribe to the topic (QoS 0) */
    emcute_sub(&emcute, &topic, 0);

    /* Start the publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_publisher, NULL, "mqtt_publisher");

    return 0;
}