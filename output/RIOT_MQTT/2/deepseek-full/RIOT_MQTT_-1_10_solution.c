
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mutex.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <dht.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <stddef.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000000

static sock_tcp_t sock;
static MQTTClient client;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

static int mqtt_read(Network* network, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_recv(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_write(Network* network, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_send(&sock, buffer, len, 0);
}

static void connect_to_broker(void) {
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create socket");
        return;
    }

    Network network = {
        .sock = sock,
        .mqttread = mqtt_read,
        .mqttwrite = mqtt_write
    };

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        puts("Failed to connect to MQTT broker");
        sock_tcp_close(&sock);
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published");
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
