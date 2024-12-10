#include <string.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define EMCUTE_ID           "esp"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define TOPIC_NAME          "state"

static emcute_topic_t mqtt_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication data of size %u\n", (unsigned)len);
}

void *pub_thread(void *arg) {
    (void)arg;
    char *message = "work or not work";
    while (1) {
        emcute_pub(&mqtt_topic, message, strlen(message), EMCUTE_QOS_0);
        sleep(5); // Publish every 5 seconds
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };

    inet_pton(AF_INET, MQTT_BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }
    puts("Successfully connected to gateway");

    mqtt_topic.name = TOPIC_NAME;

    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return 1;
    }

    printf("Subscribed to topic: %s\n", TOPIC_NAME);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    emcute_run(MQTT_BROKER_PORT, EMCUTE_ID);

    return 0;
}