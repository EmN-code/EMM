
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
#include "paho_mqtt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

static void network_init(void) {
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
}

static int connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&network.sock, &remote, &local, 0) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    return 0;
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init();
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    if (connect_to_broker() < 0) {
        return -1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_tcp_close(&network.sock);

    return 0;
}
