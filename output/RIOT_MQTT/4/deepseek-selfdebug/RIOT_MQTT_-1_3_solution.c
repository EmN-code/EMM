
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define BUF_SIZE 1024

static sock_tcp_t mqtt_sock;
static char rx_buf[BUF_SIZE];
static char tx_buf[BUF_SIZE];

static void mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    int res = sock_tcp_create(&mqtt_sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating TCP socket");
        return;
    }

    mqtt_connect_t conn_data = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    mqtt_connect_serialize(tx_buf, &conn_data);
    sock_tcp_send(mqtt_sock, tx_buf, strlen(tx_buf));
}

static void mqtt_publish(const char *topic, const char *message)
{
    mqtt_publish_t pub_data = {
        .topic = topic,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_publish_serialize(tx_buf, &pub_data);
    sock_tcp_send(mqtt_sock, tx_buf, strlen(tx_buf));
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
