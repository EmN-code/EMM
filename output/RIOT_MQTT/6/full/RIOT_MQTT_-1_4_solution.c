#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID    "esp32_client"
#define TOPIC        "state"
#define PUBLISH_MSG  "work"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void on_connect(void* context, MQTTAsync_successData* response) {
    printf("Connected to broker\n");
}

void on_connect_failure(void* context, MQTTAsync_failureData* response) {
    printf("Connection to broker failed\n");
}

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)PUBLISH_MSG;
    message.payloadlen = strlen(PUBLISH_MSG);

    int rc;
    if ((rc = MQTTPublish(client, TOPIC, &message)) != 0) {
        printf("Error publishing: %d\n", rc);
    }
    else {
        printf("Message published: %s\n", PUBLISH_MSG);
    }
}

int main(void) {
    NetworkInit(&network);

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network Connection failed: %d\n", rc);
            xtimer_usleep(PUBLISH_INTERVAL);
            continue;
        }

        MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

        MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
        connectData.MQTTVersion = 3;
        connectData.clientID.cstring = CLIENT_ID;

        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connect failed: %d\n", rc);
            NetworkDisconnect(&network);
            xtimer_usleep(PUBLISH_INTERVAL);
            continue;
        }

        while (1) {
            publish_state(&client);
            xtimer_usleep(PUBLISH_INTERVAL);
        }

        NetworkDisconnect(&network);
    }

    return 0;
}