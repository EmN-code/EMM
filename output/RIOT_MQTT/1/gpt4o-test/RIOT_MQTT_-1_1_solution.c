/*
This program establishes a connection between ESP32 and an MQTT broker, using the RIOT OS on ESP32. It will publish a state message ("work" or "not work") every 5 seconds to the configured MQTT broker at IP 47.102.103.1 using port 1883. 
*/

#include <unistd.h>  // for usleep
#include <stdio.h>   // standard I/O operations
#include <stdlib.h>  // standard library for memory allocation
#include <string.h>  // for string operations
#include <MQTTClient.h>  // for MQTT client functionality

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32_Client"
#define TOPIC "esp32/status"
#define QOS 1
#define TIMEOUT 1000L

void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    
    int rc;
    if ((rc = MQTTPublish(client, TOPIC, &message)) != 0) {
        printf("Error publishing message: %d\n", rc);
    }
    else {
        printf("Message published: %s\n", state);
    }
}

int main() {
    MQTTClient client;
    Network network;

    // MQTT buffer setup
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the network
    int rc;
    if ((rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT)) != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return EXIT_FAILURE;
    }

    // Connect to the broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to broker: %d\n", rc);
        return EXIT_FAILURE;
    }

    const char *state[] = {"work", "not work"};
    int counter = 0;
    while (1) {
        publish_state(&client, state[counter % 2]);
        counter++;
        usleep(5000000); // sleep for 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return EXIT_SUCCESS;
}