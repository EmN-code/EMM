
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <paho_mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN false

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
static MQTTClient client;
static Network network;

static void mqtt_read(Network *n, unsigned char *read_buffer, int len, int timeout)
{
    sock_udp_recv(&sock, read_buffer, len, timeout, NULL);
}

static void mqtt_write(Network *n, unsigned char *send_buffer, int len, int timeout)
{
    sock_udp_send(&sock, send_buffer, len, &server);
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAIN;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = sizeof(PAYLOAD) - 1;

    MQTTPublish(&client, TOPIC, &message);
}

static void event_callback(void *arg)
{
    (void)arg;
    publish_state();
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    evtimer_t et;
    evtimer_init(&et);

    evtimer_msg_event_t event;
    event.event.handler = event_callback;
    event.event.arg = NULL;

    evtimer_add_msg(&et, &event, thread_getpid());

    while (1) {
        xtimer_sleep(5);
        evtimer_event_send(&et, &event.event);
    }

    MQTTDisconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
