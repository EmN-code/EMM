
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 1000

static void mqtt_connect(MQTTClient *client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    int rc = MQTTConnect(client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = MQTT_DUP;
    message.id = MQTT_ID;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    printf("Starting MQTT client\n");

    Network network;
    MQTTClient client;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return 1;
    }

    mqtt_connect(&client);

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
