#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <mqttclient.h>

#define MAIN_QUEUE_SIZE     (8)
#define MQTT_TOPIC_MAXLEN   (64)
#define MQTT_MESSAGE_SIZE   (64)
#define MQTT_PUBLISH_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char mqtt_topic[MQTT_TOPIC_MAXLEN] = "state";
static char mqtt_message[MQTT_MESSAGE_SIZE] = "work";

static void *mqtt_publish_thread(void *arg) {
    (void)arg;

    while (1) {
        int res = emcute_pub(&topic, mqtt_message, strlen(mqtt_message), EMCUTE_QOS_1);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish MQTT message\n");
        }
        sleep(MQTT_PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT-SN publish example\n");

    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN gateway\n");
        return 1;
    }

    strncpy(topic.name, mqtt_topic, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register MQTT-SN topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL, "mqtt_pub_thread");

    emcute_run(MAIN_QUEUE_SIZE, "mqtt_client");

    return 0;
}