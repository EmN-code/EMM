
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    mqtt_client_init(&mqtt_client);
    mqtt_client.sendbuf = sendbuf;
    mqtt_client.sendbuf_size = sizeof(sendbuf);
    mqtt_client.readbuf = readbuf;
    mqtt_client.readbuf_size = sizeof(readbuf);

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_create(&sock, &local, NULL, 0);

    mqtt_connect_params_t conn_params = {
        .keep_alive = 60,
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL,
    };

    sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    mqtt_client_connect(&mqtt_client, &broker, &conn_params);
}

void mqtt_publish(void) {
    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
        .message_length = strlen(MQTT_MESSAGE),
    };

    mqtt_client_publish(&mqtt_client, &pub_params);
}

int main(void) {
    puts("MQTT publisher starting...");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
