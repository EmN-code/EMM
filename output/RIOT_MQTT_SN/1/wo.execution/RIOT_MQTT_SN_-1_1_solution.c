#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"

#define MQTT_SN_SERVER_IP     "47.102.103.1"
#define MQTT_SN_SERVER_PORT   1883
#define MQTT_TOPIC            "device/state"
#define INTERVAL              (5U * US_PER_SEC)

static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    char *message = "work";

    while (1) {
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        xtimer_sleep(INTERVAL/US_PER_SEC);
        if (strcmp(message, "work") == 0) {
            message = "not work";
        } else {
            message = "work";
        }
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  publish_thread, NULL, "publish");

    /* Connect to the MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_SERVER_PORT };
    inet_pton(AF_INET, MQTT_SN_SERVER_IP, &gw.addr.ipv4);

    emcute_connect(&gw, true, NULL, NULL, 0, 0);

    /* Set up the topic */
    topic.name = MQTT_TOPIC;
    topic.id = 0;

    /* Start publishing thread */
    char stack[THREAD_STACKSIZE_MAIN];
    publish_thread(NULL);

    return 0;
}