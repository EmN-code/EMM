
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static void mqtt_publish(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage msg;
    msg.qos = MQTT_QOS;
    msg.retained = MQTT_RETAIN;
    msg.dup = 0;
    msg.id = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    MQTTPublish(client, topic, &msg);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    // Initialize network
    NetworkInit(&network);

    // Connect to MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Set MQTT connection options
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    // Connect to MQTT broker
    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
