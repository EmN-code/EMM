
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <sys/socket.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000L

static sock_tcp_t sock;
static MQTTClient client;
static Network network;

int mqttread(Network *n, unsigned char *buffer, int len, int timeout) {
    return sock_tcp_read(&sock, buffer, len, timeout);
}

int mqttwrite(Network *n, unsigned char *buffer, int len, int timeout) {
    return sock_tcp_write(&sock, buffer, len, timeout);
}

void connect_to_broker(void) {
    struct sock_tcp_ep ep = { .family = AF_INET };
    inet_pton(AF_INET, BROKER_IP, &ep.addr.ipv4);
    ep.port = htons(BROKER_PORT);

    if (sock_tcp_connect(&sock, &ep, 0, 0) < 0) {
        puts("Error: Could not connect to broker");
        return;
    }

    network.mqttread = mqttread;
    network.mqttwrite = mqttwrite;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: Could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void) {
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

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        sleep(5);
    }

    return 0;
}
