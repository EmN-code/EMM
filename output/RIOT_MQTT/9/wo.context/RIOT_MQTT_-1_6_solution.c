#include "net/sock/tcp.h"
#include "net/sock.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)

int main(void) {
    // Initialize the network
    sock_tcp_t sock;
    sock_tcp_ep_t remote;

    // Set remote address
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    remote.addr.ipv4.u8[0] = 47;
    remote.addr.ipv4.u8[1] = 102;
    remote.addr.ipv4.u8[2] = 103;
    remote.addr.ipv4.u8[3] = 1;

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Error: unable to connect to the MQTT broker\n");
        return 1;
    }

    // MQTT client setup
    unsigned char sendbuf[80], readbuf[80];
    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, MQTT_PUBLISH_INTERVAL, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) < 0) {
        puts("Error: unable to connect to MQTT broker\n");
        return 1;
    }

    MQTTMessage message;
    char payload[30];

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;

    while (1) {
        strcpy(payload, "work");
        message.payloadlen = strlen(payload);

        if (MQTTPublish(&client, "esp32/status", &message) < 0) {
            puts("Error: failed to publish message\n");
        }

        xtimer_sleep(MQTT_PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}