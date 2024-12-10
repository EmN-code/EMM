#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <MQTTClient.h>

#define QOS_LEVEL 1
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "ESP32_client"
#define TOPIC "esp32/state"
#define PUBLISH_INTERVAL (5U * 1000LU * 1000LU) // 5 seconds in microseconds

int main(void)
{
    int rc;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize Network structure
    NetworkInit(&network);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Attempts connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Failed to connect to network. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully.\n");
            break;
        }
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    // Attempts MQTT connection
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("Failed to connect to MQTT broker. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker successfully.\n");
            break;
        }
    }

    while (1) {
        char *msg = (rand() % 2 == 0) ? "work" : "not work";
        MQTTMessage message;
        message.qos = QOS_LEVEL;
        message.retained = 0;
        message.payload = msg;
        message.payloadlen = strlen(msg);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message.\n");
        } else {
            printf("Message published successfully: %s\n", msg);
        }

        xtimer_sleep(5);
    }

    return 0;
}
