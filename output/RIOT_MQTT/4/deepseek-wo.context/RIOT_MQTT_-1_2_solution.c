
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_CLIENT_ID "esp32_client"

static Network network;
static MQTTClient mqttClient;

static int mqtt_connect(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return rc;
    }

    if ((rc = MQTTConnect(&mqttClient, &options)) != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return rc;
    }

    printf("MQTT connected\n");
    return 0;
}

static int mqtt_publish(const char *topic, const char *payload) {
    int rc;
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    if ((rc = MQTTPublish(&mqttClient, topic, &message)) != 0) {
        printf("MQTT publish failed: %d\n", rc);
        return rc;
    }

    printf("MQTT message published\n");
    return 0;
}

static void mqtt_disconnect(void) {
    NetworkDisconnect(&network);
    MQTTDisconnect(&mqttClient);
    printf("MQTT disconnected\n");
}

static void mqtt_network_init(void) {
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
    network.sock = SOCK_ADDR_ANY_NETIF;
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_network_init();
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    if (mqtt_connect() != 0) {
        return 1;
    }

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }

    mqtt_disconnect();
    return 0;
}
