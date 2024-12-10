
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static void network_init(Network *network) {
    network->mqttread = (int (*)(Network *, unsigned char *, int, int))sock_udp_recv;
    network->mqttwrite = (int (*)(Network *, unsigned char *, int, int))sock_udp_send;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize the network
    network_init(&network);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up the connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    // Set up the remote endpoint
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);
    remote.port = BROKER_PORT;
    remote.family = AF_INET;

    // Connect to the MQTT broker
    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Set up the message
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    // Publish the message every 5 seconds
    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
