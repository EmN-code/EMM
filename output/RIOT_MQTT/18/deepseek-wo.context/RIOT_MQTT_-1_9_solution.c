
#include <stdio.h>
#include <string.h>
#include "net/sock/tcp.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "net/mqtt.h"
#include "net/mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static msg_t msg_queue[8];
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return NULL;
    }

    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];

    network.sock = sock;
    network.mqttread = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_read;
    network.mqttwrite = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_write;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return NULL;
    }

    puts("Connected to MQTT broker");

    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Error: failed to publish message");
        } else {
            puts("Message published");
        }

        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
