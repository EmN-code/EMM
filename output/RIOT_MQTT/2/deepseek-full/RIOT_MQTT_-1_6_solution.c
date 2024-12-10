
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <msg.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT_MS 5000

static sock_tcp_t sock;
static MQTTClient client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

static int mqtt_read(struct Network* n, unsigned char* buf, int len, int timeout_ms)
{
    (void)timeout_ms;
    return sock_tcp_read(&sock, buf, len, 0);
}

static int mqtt_write(struct Network* n, unsigned char* buf, int len, int timeout_ms)
{
    (void)timeout_ms;
    return sock_tcp_send(&sock, buf, len, 0);
}

static void connect_to_broker(void)
{
    struct sock_tcp_ep remote;
    remote.family = AF_INET;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create socket");
        return;
    }

    Network network = {
        .sock = sock,
        .mqttread = mqtt_read,
        .mqttwrite = mqtt_write
    };

    MQTTClientInit(&client, &network, TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
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
    } else {
        puts("Message published successfully");
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
