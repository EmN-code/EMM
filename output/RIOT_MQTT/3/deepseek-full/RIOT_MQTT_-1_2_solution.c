
#include "net/sock.h"
#include "net/ipv4/addr.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 100
#define SEND_BUF_SIZE 256
#define READ_BUF_SIZE 256
#define COMMAND_TIMEOUT_MS 10000

static sock_udp_t sock;
static Network network;
static MQTTClient mqttClient;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void network_connect(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    network.socket = &sock;
    network.connect = sock_udp_connect;
    network.read = sock_udp_recv;
    network.write = sock_udp_send;
}

void mqtt_connect(void) {
    MQTTClientInit(&mqttClient, &network, COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&mqttClient, &data) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)"work";
    message.payloadlen = 4;

    if (MQTTPublish(&mqttClient, "state", &message) != 0) {
        puts("Error publishing message");
        return;
    }

    puts("Message published");
}

int main(void) {
    network_connect();
    mqtt_connect();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
