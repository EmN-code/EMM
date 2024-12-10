
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(void)
{
    struct sockaddr_in broker_addr;
    sock_tcp_ep_t ep;
    sock_tcp_t sock;
    mqtt_client_t client;
    mqtt_connect_info_t conn_info;

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    ep.family = AF_INET;
    ep.port = htons(MQTT_BROKER_PORT);
    ep.netif = SOCK_ADDR_ANY_NETIF;
    ep.addr.ipv4 = broker_addr.sin_addr;

    sock_tcp_create(&sock, &ep, NULL, 0);
    sock_tcp_connect(&sock, &ep, 0);

    mqtt_client_init(&client, &sock);

    conn_info.client_id = MQTT_CLIENT_ID;
    conn_info.keep_alive = MQTT_KEEPALIVE;
    conn_info.clean_session = 1;
    conn_info.will_topic = MQTT_WILL_TOPIC;
    conn_info.will_message = MQTT_WILL_MESSAGE;
    conn_info.will_qos = MQTT_WILL_QOS;
    conn_info.will_retain = MQTT_WILL_RETAIN;
    conn_info.username = MQTT_USERNAME;
    conn_info.password = MQTT_PASSWORD;

    mqtt_connect(&client, &conn_info);
}

static void mqtt_publish(void)
{
    mqtt_client_t client;
    mqtt_publish_info_t pub_info;

    pub_info.topic = MQTT_TOPIC;
    pub_info.message = MQTT_MESSAGE;
    pub_info.qos = MQTT_QOS;
    pub_info.retain = MQTT_RETAIN;

    mqtt_publish(&client, &pub_info);
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
