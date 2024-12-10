
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

static MQTTClient mqtt_client;
static Network mqtt_network;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize MQTT network
    NetworkInit(&mqtt_network);

    // Connect to MQTT broker
    if (NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker with client ID
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
