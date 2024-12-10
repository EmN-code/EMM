
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static sock_udp_t sock;
static sock_udp_ep_t broker_ep = SOCK_IPV4_EP_ANY;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

static void connect_to_broker(void)
{
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    broker_ep.port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &broker_ep.addr.ipv4);

    sock_udp_create(&sock, NULL, &broker_ep, 0);

    network.mqttread = (int (*)(Network *, unsigned char *, int, int))sock_udp_recv;
    network.mqttwrite = (int (*)(Network *, unsigned char *, int, int))sock_udp_send;

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published\n");
    }
}

static void disconnect_from_broker(void)
{
    MQTTDisconnect(&client);
    sock_udp_close(&sock);
    printf("Disconnected from MQTT broker\n");
}

static void timer_callback(void *arg)
{
    (void)arg;
    publish_state();
}

int main(void)
{
    connect_to_broker();

    evtimer_t evtimer;
    evtimer_event_t event;
    evtimer_init(&evtimer, timer_callback, NULL);

    ztimer_set(ZTIMER_MSEC, &event.event.timer, 5000);

    while (1) {
        evtimer_event_t *e;
        if ((e = evtimer_pop(&evtimer)) != NULL) {
            ztimer_set(ZTIMER_MSEC, &e->event.timer, 5000);
        }
        thread_yield();
    }

    disconnect_from_broker();

    return 0;
}
