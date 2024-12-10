#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "timer.h"  // Include RIOT's timer header for timer functionalities
#include "net/sock/tcp.h"  // Include RIOT's TCP socket header
#include "MQTTClient.h"  // Correctly place the MQTTClient header last

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);
}

int main() {
    const char *mqtt_host = "47.102.103.1";
    int mqtt_port = 1883;
    MQTTClient client;
    Network network;
    unsigned char send_buf[100], read_buf[100];
    int rc;

    // Initialize network
    NetworkInit(&network);
    
    // Connect to the network
    if ((rc = NetworkConnect(&network, (char*)mqtt_host, mqtt_port)) != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, send_buf, sizeof(send_buf), read_buf, sizeof(read_buf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_client";

    // Connect to the MQTT broker
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    // Create MQTT message
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");

    // Publish the message periodically
    while (1) {
        if ((rc = MQTTPublish(&client, "state_topic", &message)) != 0) {
            printf("Failed to publish message: %d\n", rc);
        }
        sleep(5);
    }

    // Disconnect from network
    // NetworkDisconnect(&network);  // Uncomment if NetworkDisconnect is implemented

    return 0;
}