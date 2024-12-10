
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static MQTTClient client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

int mqtt_network_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_network_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&n->sock, buffer, len);
}

void mqtt_network_init(Network *n)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&n->sock, &local, &remote, 0);
    n->mqttread = mqtt_network_read;
    n->mqttwrite = mqtt_network_write;
}

void mqtt_client_init(MQTTClient *c)
{
    MQTTClientInit(c, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

int main(void)
{
    msg_init_queue(msg_task_queue, MSG_TASK_QUEUE_SIZE);

    mqtt_network_init(&network);
    mqtt_client_init(&client);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
