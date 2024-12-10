
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include <msg.h>
#include <stdbool.h>
#include <time_units.h>
#include <stddef.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAINED false
#define KEEP_ALIVE 60
#define COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100

static sock_tcp_t sock;
static MQTTClient client;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

int mqttread(struct Network* network, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_recv(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqttwrite(struct Network* network, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_send(&sock, buffer, len, 0);
}

void connect_to_broker(void) {
    struct sock_tcp_ep remote = { .family = AF_INET };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = HTONS(SERVER_PORT);

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create socket");
        return;
    }

    Network network = {
        .sock = sock,
        .mqttread = mqttread,
        .mqttwrite = mqttwrite
    };

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEP_ALIVE;
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
    message.retained = RETAINED;
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
        xtimer_sleep(5);
    }

    return 0;
}
