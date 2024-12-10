
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient client;

void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_esp32";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    // Initialize network
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network.sock, &local, &remote, 0);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Published: %s\n", PAYLOAD);
        }

        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&client);
    printf("Disconnected from MQTT broker\n");

    return 0;
}
