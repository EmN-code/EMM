#include <xtimer.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <net/emcute.h>

#define EMCUTE_ID          ("geruehlsitzeieseirehr")
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC     "status"
#define MQTT_TOPIC_ID  1
#define MQTT_PORT      1883
#define MQTT_SERVER    "47.102.103.1"
#define PUB_INTERVAL   (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static msg_t queue[8];
static char buffer[128];  // Buffer to hold publish message

void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, queue, ARRAY_SIZE(queue));
    return NULL;  // Should never be reached
}

static void publish_state(void) {
    strcpy(buffer, "work");  // or "not work", depending on state
    emcute_pub_t pub;
    pub.topic = &topic;
    pub.data = buffer;
    pub.len = strlen(buffer);
    pub.flags = EMCUTE_QOS_0;
    
    if (emcute_pub(&pub) != EMCUTE_OK) {
        puts("error: unable to publish data");
    } else {
        puts("Published state");
    }
}

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    /* Create EMCUTE thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    sock_udp_ep_t gw = {.family = AF_INET6, .port = MQTT_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_SERVER);
    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return 1;
    }

    /* Setup the subscription topic */
    topic.name = MQTT_TOPIC;
    topic.id = MQTT_TOPIC_ID;

    /* Create publish thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "mqttsn-pub");

    return 0;
}
