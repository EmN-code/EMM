
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <string.h>
#include <MQTTClient.h>  // Ensure this is the last header file to be included

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "riot_esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqtt_client;

void network_init(void) {
    network.mqttread = NULL;
    network.mqttwrite = NULL;
}

int network_connect(Network *n, const char *address_ip, int port_number) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    sock_tcp_t sock;

    remote.family = AF_INET;
    remote.port = port_number;
    inet_pton(AF_INET, address_ip, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        return -1;
    }

    n->sock = sock;
    return 0;
}

int mqtt_publish(MQTTClient* client, const char* topic, const char* payload, int payload_len) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = payload_len;

    return MQTTPublish(client, topic, &message);
}

int main(void) {
    network_init();
    if (network_connect(&network, BROKER_IP, BROKER_PORT) < 0) {
        puts("Failed to connect to network");
        return 1;
    }

    MQTTClientInit(&mqtt_client, &network, 1000, NULL, 0, NULL, 0);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        if (mqtt_publish(&mqtt_client, TOPIC, PAYLOAD, PAYLOAD_LEN) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
