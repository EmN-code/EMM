
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static struct Network network;
static MQTTClient client;

static int mqtt_read(struct Network* n, unsigned char* buffer, int len, int timeout)
{
    return sock_tcp_read(&n->sock, buffer, len, timeout);
}

static int mqtt_write(struct Network* n, unsigned char* buffer, int len, int timeout)
{
    return sock_tcp_send(&n->sock, buffer, len, timeout);
}

static void connect_to_broker(void)
{
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&network.sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create socket");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
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
        puts("Failed to publish message");
        return;
    }

    puts("Message published");
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    sock_tcp_close(&network.sock);
    return 0;
}
