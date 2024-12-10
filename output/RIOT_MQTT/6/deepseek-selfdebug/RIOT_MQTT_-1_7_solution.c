
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t mqtt_sock;
static sock_udp_ep_t mqtt_remote;

static void mqtt_connect(void)
{
    int result;
    result = inet_pton(AF_INET, MQTT_BROKER_IP, &mqtt_remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return;
    }
    mqtt_remote.port = MQTT_BROKER_PORT;
    mqtt_remote.family = AF_INET;

    result = sock_udp_create(&mqtt_sock, NULL, &mqtt_remote, 0);
    if (result < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

static void mqtt_publish(void)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    int result = mqtt_publish(&mqtt_sock, &pub, &mqtt_remote);
    if (result < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
