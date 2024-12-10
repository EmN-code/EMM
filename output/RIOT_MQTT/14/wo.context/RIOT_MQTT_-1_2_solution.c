#include <string.h>
#include <stdio.h>
#include "MQTTClient.h"  // Ensure this is the last header file included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// Network and MQTT client
Network network;
MQTTClient client;

// Buffers for MQTT communication
unsigned char sendbuf[80], readbuf[80];

void message_handler(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data, 
           data->message->payloadlen, (char*)data->message->payload);
}

// Function prototype for connect_to_broker
int connect_to_broker(void);

// Connect function
int connect_to_broker(void) {
    int rc;
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return rc;
    }
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";
    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return rc;
    }
    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    return 0;
}

int main(void) {
    printf("MQTT example for RIOT-ESP32\n");
    int rc;
    rc = connect_to_broker();
    if (rc != 0) {
        return 1;  // Return error
    }
    // Subscribe to a topic
    MQTTSubscribe(&client, "test/topic", QOS0, message_handler);
    // Main loop: publish message every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work or not work";
        message.payloadlen = strlen("work or not work");
        rc = MQTTPublish(&client, "test/topic", &message);
        if (rc != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Message published successfully\n");
        }
        sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}