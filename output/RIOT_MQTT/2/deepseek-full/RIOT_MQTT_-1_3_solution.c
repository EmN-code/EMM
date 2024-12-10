
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <msg.h>
#include <ztimer.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_esp32"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static sock_tcp_t _sock;
static Network network;
static MQTTClient client;

static int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(&_sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&_sock, buffer, len, 0);
}

static void connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&_sock, &remote, &local, 0) < 0) {
        puts("Error: Could not connect to server");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: Could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error: Could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
