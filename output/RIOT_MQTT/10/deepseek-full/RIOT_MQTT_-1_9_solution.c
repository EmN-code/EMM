
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <timex.h>
#include <stdbool.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

static void mqtt_read(Network *n, unsigned char *read_buffer, int len, int timeout)
{
    sock_udp_recv(&sock, read_buffer, len, timeout, NULL);
}

static void mqtt_write(Network *n, unsigned char *send_buffer, int len, int timeout)
{
    sock_udp_send(&sock, send_buffer, len, &server);
}

static void connect_to_broker(void)
{
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_create(&sock, NULL, &server, 0);

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
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
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published message: %s\n", PAYLOAD);
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

    evtimer_t timer;
    evtimer_event_t event;

    evtimer_init(&timer);
    event.event.callback = timer_callback;
    event.event.arg = NULL;

    while (1) {
        evtimer_add(&timer, &event.event, (timex_t){5, 0});
        xtimer_sleep(5);
    }

    disconnect_from_broker();

    return 0;
}
