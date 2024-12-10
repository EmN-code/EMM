
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_CLIENT_ID "esp32_client"

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    Network network;
    MQTTClient client;

    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, NULL, NULL);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
