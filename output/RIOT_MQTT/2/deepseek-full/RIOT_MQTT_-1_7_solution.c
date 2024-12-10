
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <dht.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000000

static sock_tcp_t sock;
static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_recv(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_send(&sock, buffer, len);
}

void connect_to_broker(void) {
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &remote, NULL, 0) < 0) {
        puts("Failed to create socket");
        return;
    }

    network.sock = sock;
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
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
        puts("Failed to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(TIMEOUT);
    }

    MQTTDisconnect(&client);
    sock_tcp_close(&sock);

    return 0;
}
