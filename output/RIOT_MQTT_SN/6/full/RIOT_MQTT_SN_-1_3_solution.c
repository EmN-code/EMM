#include "netinet/in.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define STATUS_MESSAGE "work"

#define MQTT_SEND_BUFF_SIZE 128
#define MQTT_READ_BUFF_SIZE 128

void on_message_delivered(void *context, MQTTMessageDeliveryToken token) {
    printf("Message delivered with token value %d\n", token);
}

void on_message_received(void *context, char *topicName, int topicLen, MQTTMessage *message) {
    printf("Message arrived on topic name: %s\n", topicName);
    printf("Message: %.*s\n", message->payloadlen, (char *)message->payload);
}

void on_connection_lost(void *context, char *cause) {
    printf("Connection lost, cause: %s\n", cause);
}

int main(void) {
    unsigned char sendbuf[MQTT_SEND_BUFF_SIZE];
    unsigned char readbuf[MQTT_READ_BUFF_SIZE];
    int rc, count = 0;

    MQTTClient client;
    Network network;

    // Initialize client and network
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Establish network connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            fprintf(stderr, "Unable to connect to network, will retry in 1 second\n");
            sleep(1);
            continue;
        }
        break;
    }

    // Prepare MQTT connect data	d MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    // Establish MQTT connection
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            fprintf(stderr, "Unable to connect to MQTT broker, will retry in 1 second\n");
            sleep(1);
            continue;
        }
        break;
    }

    printf("Connected to MQTT broker\n");

    // Prepare MQTT message
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *) STATUS_MESSAGE;
    message.payloadlen = strlen(STATUS_MESSAGE);

    // Publishing loop
    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            fprintf(stderr, "Failed to publish message, will retry\n");
        } else {
            printf("Published work status\n");
        }

        // Wait 5 seconds until next publish
        sleep(5);
    }

    // Cleanup
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return EXIT_SUCCESS;
}