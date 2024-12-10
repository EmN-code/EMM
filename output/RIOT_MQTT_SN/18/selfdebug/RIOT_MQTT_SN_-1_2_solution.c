#include "xtimer.h"
#include <stdio.h>
#include "net/emcute.h"
#include "msg.h"

#define EMCUTE_ID           "device"
#define MQTT_SN_BROKER_IP   "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC       "state"
#define INTERVAL            5000000    // 5 seconds in microseconds

static emcute_topic_t topic;
static msg_t queue[16];

static void on_mqtt_event(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received data: %.*s\n", (int)len, (char *)data);
}

static void publish_state(void) {
    const char *state = "work";  // Change this to "not work" at runtime based on condition
    printf("Publishing: %s\n", state);
    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_1);
    if (res != EMCUTE_OK) {
        printf("Publish error: %d\n", res);
    }
}

void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_topic_t topic = {
        .name = MQTT_SN_TOPIC,
        .id = 0
    };

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_SN_BROKER_PORT
    };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("EMCUTE: Could not connect to gateway\n");
        return 1;
    }
    printf("Connected to the MQTT-SN broker at %s:%d\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);

    if (emcute_sub(&topic, EMCUTE_QOS_1) != EMCUTE_OK) {
        printf("EMCUTE: Could not subscribe to topic\n");
        return 1;
    }

    printf("Subscribed to topic: %s\n", MQTT_SN_TOPIC);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher_thread, NULL, "publisher");

    emcute_run(MQTT_SN_BROKER_PORT, EMCUTE_ID);
    return 0;
}