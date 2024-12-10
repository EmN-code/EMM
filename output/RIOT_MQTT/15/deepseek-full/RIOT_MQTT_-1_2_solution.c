
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL 5  // in seconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buf[128];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    // Create a UDP socket
    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Initialize MQTT client
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];
    int rc = 0;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_Client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published successfully\n");
        }

        sleep(INTERVAL);
    }

    // Clean up
    MQTTClose(&client);
    sock_udp_close(&sock);

    return 0;
}
