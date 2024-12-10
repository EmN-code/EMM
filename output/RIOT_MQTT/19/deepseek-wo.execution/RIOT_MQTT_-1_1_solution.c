
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
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
    local.port = 0;
    sock_udp_create(&sock, &local, NULL, 0);

    sock_udp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    mqtt_client_connect(&mqtt_client, &broker, MQTT_CLIENT_ID, NULL, NULL, 0, 0);
}

void mqtt_publish(void) {
    mqtt_publish_t pubmsg;
    pubmsg.topic = MQTT_TOPIC;
    pubmsg.payload = MQTT_MESSAGE;
    pubmsg.payload_len = MQTT_MESSAGE_LEN;
    pubmsg.qos = MQTT_QOS_0;
    pubmsg.retain = 0;
    mqtt_publish(&mqtt_client, &pubmsg);
}

int main(void) {
    puts("MQTT publisher starting...");

    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
