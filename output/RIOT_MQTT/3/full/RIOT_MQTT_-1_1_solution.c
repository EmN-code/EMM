#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt_client.h"
#include "net/af.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE_MESSAGE "work or not work"
#define PUB_INTERVAL (5U * 1000000U)

static mqtt_client_t client;
static sock_udp_ep_t broker_endpoint;
static xtimer_ticks32_t last_pub_time;

void publish_state_msg(void) {
    mqtt_message_t msg;
    msg.payload = (void *)STATE_MESSAGE;
    msg.payloadlen = strlen(STATE_MESSAGE);
    msg.qos = MQTTQOS0;

    int res = MQTTPublish(&client, STATE_TOPIC, &msg);
    if (res != 0) {
        printf("Failed to publish message, reason: %d\n", res);
    } else {
        printf("Published: %s\n", STATE_MESSAGE);
    }
}

int main(void) {
    broker_endpoint.family = AF_INET;
    broker_endpoint.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr.ipv4);
    broker_endpoint.port = BROKER_PORT;

    mqtt_client_init(&client, &broker_endpoint, "esp32_client");

    if (MQTTConnect(&client) != 0) {
        printf("MQTT connection failed\n");
        return 1;
    }

    last_pub_time = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_pub_time, PUB_INTERVAL);
        publish_state_msg();
    }

    MQTTDisconnect(&client);
    return 0;
}