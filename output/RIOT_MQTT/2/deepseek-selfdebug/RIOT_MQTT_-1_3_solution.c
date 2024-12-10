
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static sock_udp_t mqtt_sock;
static sock_udp_ep_t broker_ep = SOCK_IPV4_EP_ANY;

static void mqtt_connect(void)
{
    broker_ep.port = MQTT_BROKER_PORT;
    broker_ep.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    if (sock_udp_create(&mqtt_sock, NULL, &broker_ep, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    mqtt_connect_msg_t conn_msg = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = true,
        .will_flag = false,
        .will_qos = 0,
        .will_retain = false,
        .username_flag = false,
        .password_flag = false,
    };

    uint8_t buf[128];
    size_t len = mqtt_connect_msg_pack(buf, sizeof(buf), &conn_msg);
    if (sock_udp_send(&mqtt_sock, buf, len, &broker_ep) < 0) {
        puts("Error sending MQTT connect message");
    }
}

static void mqtt_publish(void)
{
    mqtt_publish_msg_t pub_msg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    uint8_t buf[128];
    size_t len = mqtt_publish_msg_pack(buf, sizeof(buf), &pub_msg);
    if (sock_udp_send(&mqtt_sock, buf, len, &broker_ep) < 0) {
        puts("Error sending MQTT publish message");
    }
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
