#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define DEVICE_ID "esp32_device"
#define TOPIC_NAME "esp32/state"
#define PUBLISH_INTERVAL (5U * 1000U) // in milliseconds

typedef struct {
    const char *name;
    uint16_t id;
} emcute_topic_t;

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;  // usually assigned by the broker, set to 0 for now
    
    const char *state = "work";
    emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    printf("Published state: %s\n", state);
}

int main(void) {
    emcute_conf_t conf;
    conf.id = DEVICE_ID;
    conf.port = MQTT_SN_BROKER_PORT;
    strcpy(conf.host, MQTT_SN_BROKER_IP);

    if (emcute_con(&conf, true, NULL, NULL) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}