#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL 5

static emcute_topic_t _mqtt_topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Handle received publication (if needed)
}

static int mqtt_sn_connect(void) {
    int res;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    res = emcute_connect(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        return res;
    }

    _mqtt_topic.name = "device/state";
    _mqtt_topic.id = 0; // Let broker assign an ID
    return res;
}

static void mqtt_sn_publish(const char *state) {
    emcute_reg_t reg;
    reg.topic = &_mqtt_topic;
    reg.cb = _on_pub;
    emcute_reg(&reg);

    emcute_publish(_mqtt_topic.name, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    puts("Starting MQTT-SN application");

    if (mqtt_sn_connect() != EMCUTE_OK) {
        puts("Failed to connect to the broker.");
        return 1;
    }

    while (1) {
        mqtt_sn_publish("work");
        xtimer_sleep(PUB_INTERVAL);
        mqtt_sn_publish("not work");
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}