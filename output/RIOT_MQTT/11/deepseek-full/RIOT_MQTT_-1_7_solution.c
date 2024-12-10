
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static volatile bool is_connected = false;

void messageArrived(MessageData* md) {
    (void)md;
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    // Initialize network
    NetworkInit(&network);

    // Connect to the MQTT broker
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    is_connected = true;

    // Publish state every 5 seconds
    while (1) {
        if (is_connected) {
            MQTTMessage message;
            message.qos = QOS;
            message.retained = 0;
            message.dup = 0;
            message.payload = (void*)PAYLOAD;
            message.payloadlen = sizeof(PAYLOAD) - 1;

            rc = MQTTPublish(&client, TOPIC, &message);
            if (rc != 0) {
                printf("Failed to publish message\n");
            } else {
                printf("Message published\n");
            }
        }
        sleep(5);
    }

    // Disconnect and clean up
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
