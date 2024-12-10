
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_CLIENT_ID "esp32_client"

static void mqtt_connect(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    mqtt_connect_msg_t conn_msg;
    mqtt_connect_msg_init(&conn_msg, &conn_info);

    uint8_t buf[128];
    size_t len = mqtt_connect_msg_pack(&conn_msg, buf, sizeof(buf));

    sock_udp_send(sock, buf, len, remote);
}

static void mqtt_publish(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_PAYLOAD,
        .payload_len = strlen(MQTT_PAYLOAD),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_publish_msg_t pub_msg;
    mqtt_publish_msg_init(&pub_msg, &pub_info);

    uint8_t buf[128];
    size_t len = mqtt_publish_msg_pack(&pub_msg, buf, sizeof(buf));

    sock_udp_send(sock, buf, len, remote);
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_connect(&sock, &remote);

    while (1) {
        mqtt_publish(&sock, &remote);
        xtimer_sleep(5);
    }

    return 0;
}
