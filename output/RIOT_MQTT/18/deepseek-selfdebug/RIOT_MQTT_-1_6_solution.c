
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void mqtt_connect(sock_tcp_t *sock)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &addr.sin_addr);

    sock_tcp_connect(sock, (struct sockaddr *)&addr, 0);
}

static void mqtt_publish(sock_tcp_t *sock)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish(sock, &pub);
}

int main(void)
{
    sock_tcp_t sock;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    sock_tcp_create(&sock, NULL, NULL, 0);
    mqtt_connect(&sock);

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        mqtt_publish(&sock);
    }

    sock_tcp_close(&sock);
    return 0;
}
