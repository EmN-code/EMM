
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static sock_tcp_ep_t broker_ep = { .family = AF_INET, .port = MQTT_BROKER_PORT };

static void _mqtt_connect(void)
{
    int res;
    sock_tcp_ep_t local = { .family = AF_INET };

    res = sock_tcp_create(&mqtt_sock, &local, &broker_ep, 0);
    if (res < 0) {
        printf("Error creating TCP socket: %d\n", res);
        return;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

static void _mqtt_publish(void)
{
    uint8_t buf[128];
    mqtt_publish_t pub;
    mqtt_packet_t packet;

    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = MQTT_PAYLOAD_LEN;
    pub.qos = 0;
    pub.retain = 0;

    mqtt_publish_to_buf(&pub, &packet, buf, sizeof(buf));

    ssize_t sent = sock_tcp_send(&mqtt_sock, buf, packet.length, 0);
    if (sent < 0) {
        printf("Error sending MQTT publish packet: %zd\n", sent);
    } else {
        printf("Published message: %s\n", MQTT_PAYLOAD);
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    _mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
        _mqtt_publish();
    }

    return 0;
}
