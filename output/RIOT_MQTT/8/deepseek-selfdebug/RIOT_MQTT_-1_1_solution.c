
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void mqtt_connect(void) {
    mqtt_client_init(&mqtt_client);
    mqtt_client.sendbuf = sendbuf;
    mqtt_client.sendbuf_size = sizeof(sendbuf);
    mqtt_client.readbuf = readbuf;
    mqtt_client.readbuf_size = sizeof(readbuf);

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client_connect(&mqtt_client, &sock, MQTT_CLIENT_ID);
}

void mqtt_publish(void) {
    mqtt_publish_t pubmsg;
    pubmsg.topic = MQTT_TOPIC;
    pubmsg.message = MQTT_MESSAGE;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retain = MQTT_RETAIN;
    pubmsg.message_len = strlen(MQTT_MESSAGE);

    mqtt_client_publish(&mqtt_client, &pubmsg);
}

int main(void) {
    puts("Starting MQTT client...");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
