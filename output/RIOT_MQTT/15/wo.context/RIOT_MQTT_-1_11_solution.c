#include "net/ipv6/addr.h"
#include "mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define STATE_TOPIC       "device/state"
#define STATE_PAYLOAD     "work"
#define NOT_STATE_PAYLOAD "not work"

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    char* payload = (char*) ((rand() % 2) ? STATE_PAYLOAD : NOT_STATE_PAYLOAD);

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(client, STATE_TOPIC, &message);

    printf("Published state: %s\n", payload);
}

int main(void) {
    MQTTClient client;
    Network network;
    int rc = 0;
    char clientID[] = "riot_client";
    unsigned char sendBuffer[100], readBuffer[100];

    // Initialize network and MQTT client
    NetworkInit(&network);

    // Connect to the network with retry mechanism
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Network connected successfully.\n");
        break;
    }

    MQTTClientInit(&client, &network, 1000, sendBuffer, sizeof(sendBuffer), readBuffer, sizeof(readBuffer));

    // Connect to MQTT Broker and handle any connection errors
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = clientID;

    while (1) {
        rc = MQTTConnect(&client, &options);
        if (rc != SUCCESS) {
            printf("MQTT connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("MQTT connected successfully.\n");
        break;
    }

    // Publish the state every 5 seconds
    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    return 0;
}
