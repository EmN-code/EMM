
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include "MQTTClient.h"

#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_YIELD_POLLING_MS (30)

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"

static sock_tcp_t sock;
static MQTTClient client;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

static void mqtt_network_init(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    int result;

    local.port = 0;
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: invalid server IP address");
        return;
    }

    if (sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to create TCP socket");
        return;
    }

    if (sock_tcp_connect(&sock, &remote, SOCK_NO_TIMEOUT) < 0) {
        puts("Error: failed to connect to server");
        return;
    }
}

static int mqtt_network_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_read(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_network_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_write(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

static void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen((char*)message.payload);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    Network network;

    puts("Starting MQTT client");

    mqtt_network_init();

    network.mqttread = mqtt_network_read;
    network.mqttwrite = mqtt_network_write;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
