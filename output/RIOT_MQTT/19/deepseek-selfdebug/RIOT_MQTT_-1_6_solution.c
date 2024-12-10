
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
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
#define MQTT_WILL_FLAG 0
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void mqtt_connect(void)
{
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    sock_tcp_create(&mqtt_sock, NULL, (struct sockaddr *)&broker_addr, SOCK_FLAGS_REUSE_EP);
    mqtt_client_init(&mqtt_client, &mqtt_sock);

    mqtt_connect_params_t connect_params = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_flag = MQTT_WILL_FLAG,
        .will = {
            .topic = MQTT_WILL_TOPIC,
            .message = MQTT_WILL_MESSAGE,
            .qos = MQTT_WILL_QOS,
            .retain = MQTT_WILL_RETAIN
        },
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD
    };

    mqtt_connect(&mqtt_client, &connect_params);
}

static void mqtt_publish(void)
{
    mqtt_publish_params_t publish_params = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    mqtt_publish(&mqtt_client, &publish_params);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        mqtt_publish();
    }

    return 0;
}
