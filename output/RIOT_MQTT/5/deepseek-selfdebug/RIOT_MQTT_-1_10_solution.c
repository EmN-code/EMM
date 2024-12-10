
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_PROTOCOL_LEVEL 4
#define MQTT_CLIENT_ID "esp32_client"

static sock_udp_t mqtt_sock;
static sock_udp_ep_t mqtt_broker = SOCK_IPV4_EP_ANY;

static void mqtt_connect(void)
{
    mqtt_broker.port = MQTT_BROKER_PORT;
    mqtt_broker.family = AF_INET;
    mqtt_broker.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &mqtt_broker.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return;
    }

    if (sock_udp_create(&mqtt_sock, NULL, &mqtt_broker, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(const char *topic, const char *message)
{
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;
    pub.dup = MQTT_DUP;
    pub.id = MQTT_ID;

    if (mqtt_publish(&mqtt_sock, &pub) < 0) {
        puts("Error: failed to publish message");
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
