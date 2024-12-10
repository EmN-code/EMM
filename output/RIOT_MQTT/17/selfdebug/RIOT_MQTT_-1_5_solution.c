#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static mqtt_client_t client;
static char mqtt_buf[512];
static char mqtt_readbuf[512];

int main(void)
{
    mqtt_client_init(&client, mqtt_buf, sizeof(mqtt_buf), mqtt_readbuf, sizeof(mqtt_readbuf), NULL, NULL);

    sock_udp_ep_t broker;
    memset(&broker, 0, sizeof(broker));
    broker.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4);

    mqtt_connect(&client, &broker, true, NULL, NULL, 0, 0, 0);

    while (1) {
        mqtt_publish(&client, false, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0);
        xtimer_sleep(PUBLISH_INTERVAL / 1000000);
    }

    return 0;
}