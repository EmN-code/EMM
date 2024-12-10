#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <net/sock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE_1 "work"
#define MQTT_MESSAGE_2 "not work"

static sock_udp_ep_t broker_ep;
static mqttsn_client_t mqtt_client;
static mqttsn_connect_param_t connect_param;

static int publish_state(const char *state) {
    mqttsn_pub_param_t pub_param;
    pub_param.topic.id = 0;
    pub_param.topic.name = MQTT_TOPIC;
    pub_param.data = state;
    pub_param.datalen = strlen(state);
    pub_param.qos = 0;
    pub_param.retain = 0;
    return mqttsn_pub(&mqtt_client, &pub_param, false);
}

int main(void) {
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);
    broker_ep.port = MQTT_BROKER_PORT;

    mqttsn_init(&mqtt_client, &broker_ep, 0);

    connect_param.clean = 1;
    connect_param.will = MQTT_NO_WILL;
    connect_param.keepalive = 30;
    connect_param.client_id = "esp32_device";

    mqttsn_connect(&mqtt_client, &connect_param, false);

    for (;;) {
        publish_state(MQTT_MESSAGE_1);
        xtimer_sleep(5);
        publish_state(MQTT_MESSAGE_2);
        xtimer_sleep(5);
    }
    
    return 0;
}