#include <string.h>
#include <stdio.h>
#include <emcute.h>
#include "thread.h"
#include "xtimer.h"

#define MQTT_BROKER_ADDR     "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUB_TOPIC            "device/state"
#define CLIENT_ID            "ESP32_Client"

static char stack[THREAD_STACKSIZE_DEFAULT];
static char buf[128];
static char topic[64];
static emcute_topic_t emcute_topic;

void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    memcpy(&gw.addr.ipv4, MQTT_BROKER_ADDR, sizeof(gw.addr.ipv4));

    /* Connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to MQTT broker\n");
        return NULL;
    }

    /* Set topic */
    strcpy(topic, PUB_TOPIC);
    emcute_topic.name = topic;

    /* Register topic */
    if (emcute_reg(&emcute_topic) != EMCUTE_OK) {
        printf("Could not register topic\n");
        return NULL;
    }

    /* Publish every 5 seconds */
    while(1) {
        const char *state = "work or not work";
        printf("Publishing to topic %s\n", emcute_topic.name);
        if (emcute_pub(&emcute_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Could not publish data\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}