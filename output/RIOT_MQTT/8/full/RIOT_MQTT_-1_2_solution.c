#include "paho_mqtt.h"
#include "MQTTClient.h"
#include <unistd.h>
#include <stdlib.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define TOPIC "state_topic"
#define PAYLOAD1 "work"
#define PAYLOAD2 "not work"
#define QOS 1
#define TIMEOUT 10000L

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    int rc;

    message.qos = QOS;
    message.retained = 0;
    message.payload = PAYLOAD1;
    message.payloadlen = strlen(PAYLOAD1);

    rc = MQTTPublish(client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish 'work' state, return code %d\n", rc);
    }

    sleep(5);
    
    message.payload = PAYLOAD2;
    message.payloadlen = strlen(PAYLOAD2);

    rc = MQTTPublish(client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish 'not work' state, return code %d\n", rc);
    }
}

int main() {
    int rc;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClient client;
    Network network;

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed with return code %d. Retrying...\n", rc);
            sleep(1);
            continue;
        }
        printf("Network connection successful.\n");
        break;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed with return code %d. Retrying...\n", rc);
            sleep(1);
            continue;
        }
        printf("MQTT connection successful.\n");
        break;
    }

    while (1) {
        publish_state(&client);
    }

    return 0;
}