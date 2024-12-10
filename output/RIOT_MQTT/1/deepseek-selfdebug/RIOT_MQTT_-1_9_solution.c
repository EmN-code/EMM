
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
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

static void mqtt_connect(sock_tcp_t *sock)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &addr.sin_addr);

    sock_tcp_connect(sock, (struct sockaddr *)&addr, SOCK_FLAGS_REUSE_EP);
}

static void mqtt_publish(sock_tcp_t *sock)
{
    mqtt_publish_params_t pub_params;
    pub_params.topic = MQTT_TOPIC;
    pub_params.message = MQTT_MESSAGE;
    pub_params.qos = MQTT_QOS;
    pub_params.retain = MQTT_RETAIN;

    mqtt_publish(sock, &pub_params);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock);
        xtimer_sleep(5);
    }

    return 0;
}
