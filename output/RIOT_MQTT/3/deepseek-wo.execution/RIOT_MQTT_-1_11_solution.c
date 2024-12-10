
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

static Network network;
static MQTTClient client;

static int mqtt_connect(void)
{
    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }

    printf("Connected to MQTT broker\n");
    return 0;
}

static int mqtt_publish(const char *topic, const char *payload)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, topic, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }

    return rc;
}

static void network_init(void)
{
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
    network.sock = SOCK_ADDR_ANY_NETIF;
}

static int network_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int rc = sock_tcp_connect(&network.sock, &remote, &local, 0);
    if (rc < 0) {
        printf("Failed to connect to server, return code %d\n", rc);
        return rc;
    }

    printf("Connected to server\n");
    return 0;
}

static void network_disconnect(void)
{
    sock_tcp_disconnect(&network.sock);
    printf("Disconnected from server\n");
}

int main(void)
{
    network_init();
    if (network_connect() != 0) {
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    if (mqtt_connect() != 0) {
        network_disconnect();
        return 1;
    }

    while (1) {
        mqtt_publish(TOPIC, PAYLOAD);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    network_disconnect();
    return 0;
}
