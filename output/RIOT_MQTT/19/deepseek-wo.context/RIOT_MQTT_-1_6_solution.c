
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "riot_esp32"

static void mqtt_connect(MQTTClient *client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(client, MQTT_TOPIC, &message) != 0) {
        puts("Failed to publish message");
        return;
    }
    puts("Message published");
}

int main(void) {
    puts("Starting MQTT example");

    Network network;
    MQTTClient client;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        puts("Failed to connect to network");
        return 1;
    }
    puts("Network connected");

    mqtt_connect(&client);

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
