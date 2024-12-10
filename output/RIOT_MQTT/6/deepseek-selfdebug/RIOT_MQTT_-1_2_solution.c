
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static struct Network mqtt_network;

static void mqtt_connect(void)
{
    sock_udp_ep_t local = SOCK_ADDR_ANY_NETIF;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    mqtt_network.sock = mqtt_sock;
    mqtt_network.mqttread = sock_tcp_read;
    mqtt_network.mqttwrite = sock_tcp_write;
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTString topicString = MQTTString_initializer;
    topicString.cstring = MQTT_TOPIC;

    int rc = MQTTConnect(&mqtt_network, NULL);
    if (rc != 0) {
        printf("MQTT connect failed with return code %d\n", rc);
        return;
    }

    rc = MQTTPublish(&mqtt_network, &topicString, &message);
    if (rc != 0) {
        printf("MQTT publish failed with return code %d\n", rc);
    } else {
        puts("MQTT message published successfully");
    }

    MQTTDisconnect(&mqtt_network);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}
