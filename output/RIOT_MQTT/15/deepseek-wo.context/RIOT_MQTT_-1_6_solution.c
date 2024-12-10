
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    network.sock = sock;
    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return 1;
    }

    printf("MQTT connected\n");

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
