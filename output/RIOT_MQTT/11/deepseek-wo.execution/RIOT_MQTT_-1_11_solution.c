
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    unsigned char sendbuf[200], readbuf[200];
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize network
    NetworkInit(&network);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    // Connect to MQTT broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        xtimer_sleep(5);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
