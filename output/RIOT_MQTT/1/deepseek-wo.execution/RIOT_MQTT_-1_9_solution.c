
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION true
#define MQTT_CLIENT_ID "riot_esp32"

#define MSG_QUEUE_SIZE 10
#define SEND_BUF_SIZE 1024
#define READ_BUF_SIZE 1024

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

static void mqtt_network_init(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&mqtt_network.sock, &local, &remote, 0);
}

static int mqtt_network_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_network_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&n->sock, buffer, len, 0);
}

static void mqtt_client_init(void)
{
    mqtt_network.mqttread = mqtt_network_read;
    mqtt_network.mqttwrite = mqtt_network_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.clientID.cstring = MQTT_CLIENT_ID;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with return code %d\n", rc);
    } else {
        printf("MQTT message published\n");
    }
}

static void mqtt_disconnect(void)
{
    int rc = MQTTDisconnect(&mqtt_client);
    if (rc != 0) {
        printf("MQTT disconnect failed with return code %d\n", rc);
    } else {
        printf("MQTT disconnected\n");
    }
}

int main(void)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    mqtt_network_init();
    mqtt_client_init();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    mqtt_disconnect();
    return 0;
}
