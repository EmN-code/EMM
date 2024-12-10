
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <ztimer.h>
#include <xtimer.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <dht.h>
#include <timex.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define QOS 0
#define RETAINED false

static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    return sock_tcp_recv(n->sock, buffer, len, timeout);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    return sock_tcp_send(n->sock, buffer, len, timeout);
}

void connect_to_broker(void) {
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

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAINED;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    puts("Starting MQTT client");

    unsigned char sendbuf[256];
    unsigned char readbuf[256];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_tcp_close(network.sock);

    return 0;
}
