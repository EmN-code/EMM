
#include <net/sock.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60

static void mqtt_message_arrived(MessageData* data) {
    (void)data;
}

int main(void) {
    // Initialize MQTT client
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_KEEPALIVE, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up the remote endpoint for the MQTT broker
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Connect to the MQTT broker
    printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.payload = (void*)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", MQTT_PAYLOAD);
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
