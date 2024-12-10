
#include <stdio.h>
#include <xtimer.h>
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
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    sock_tcp_t sock;
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    // Initialize the network
    NetworkInit(&network);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Set the MQTT connection options
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    // Connect to the MQTT broker
    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish the state every 5 seconds
    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    NetworkDisconnect(&network);

    return 0;
}
