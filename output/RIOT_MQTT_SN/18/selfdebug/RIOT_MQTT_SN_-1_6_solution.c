#include <stdio.h>
#include <net/ipv4/addr.h>
#include "emcute.h"
#include "thread.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define SLEEP_TIME 5

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *mqtt_client_thread(void *arg) {
    (void)arg;

    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr) <= 0) {
        puts("ERROR: Converting IP address failed!");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: Unable to connect to MQTT broker!");
        return NULL;
    }

    topic.name = MQTT_TOPIC;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: Unable to register topic!");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), EMCUTE_QOS_0) == EMCUTE_OK) {
            puts("Published successfully!");
        } else {
            puts("Publish failed!");
        }
        sleep(SLEEP_TIME);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");
    return 0;
}