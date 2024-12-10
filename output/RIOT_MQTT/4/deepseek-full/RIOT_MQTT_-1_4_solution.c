
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <stdbool.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <stdio.h>
#include <thread.h>
#include <stdlib.h>
#include <MQTTClient.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static volatile bool done = false;

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);
}

void connect_lost(void* context, char* cause) {
    printf("Connection lost: %s\n", cause);
    done = true;
}

int main(void) {
    MQTTClient client;
    Network network;
    int rc;

    // Initialize the network
    NetworkInit(&network);

    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, BROKER_ADDR);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    // Set the connection options
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    // Connect to the MQTT broker
    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Set the message arrived and connection lost callbacks
    client.messageArrived = messageArrived;
    client.connectionLost = connect_lost;

    // Publish a message every 5 seconds
    while (!done) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
    printf("Disconnected from MQTT broker\n");

    return 0;
}
