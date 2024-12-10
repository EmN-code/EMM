
#include <net/sock.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include <stdatomic.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <dht.h>
#include <ztimer.h>
#include <stddef.h>
#include <net/sock/udp.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_tcp_t sock;
static MQTTClient client;
static Network network;

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&sock, buffer, len);
}

void connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating socket");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error publishing message");
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

    sock_tcp_close(&sock);
    return 0;
}
