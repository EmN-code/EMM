
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 10000

static Network network;
static MQTTClient mqttClient;

static void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT message published successfully\n");
    }
}

static void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
    } else {
        printf("MQTT connected successfully\n");
    }
}

static void network_connect(void) {
    int rc = network.connect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT, SOCK_STREAM);
    if (rc != 0) {
        printf("Network connect failed with error code %d\n", rc);
    } else {
        printf("Network connected successfully\n");
    }
}

int main(void) {
    printf("Starting MQTT client on RIOT for ESP32\n");

    network.connect = sock_tcp_connect;
    network.disconnect = sock_tcp_disconnect;
    network.read = sock_tcp_read;
    network.write = sock_tcp_write;

    MQTTClientInit(&mqttClient, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    network_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
