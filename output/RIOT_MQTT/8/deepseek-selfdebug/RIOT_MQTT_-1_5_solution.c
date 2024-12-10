
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "riot_esp32"

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t broker_ep = SOCK_IPV4_EP_ANY;

static void mqtt_connect(void)
{
    broker_ep.port = MQTT_BROKER_PORT;
    broker_ep.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &broker_ep, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(void)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = strlen(MQTT_PAYLOAD);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    if (mqtt_publish(&sock, &pub) < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
