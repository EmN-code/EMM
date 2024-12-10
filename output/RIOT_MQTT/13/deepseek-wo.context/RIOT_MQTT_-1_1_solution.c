
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
static sock_udp_t sock;
static mqtt_client_t client;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void mqtt_connect(void) {
    inet_addr_from_str(broker.addr.ipv4, MQTT_BROKER_IP);
    sock_udp_create(&sock, NULL, &broker, 0);

    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_msg_t connect_msg = {
        .keep_alive = 60,
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .will_flag = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_client_connect(&client, &connect_msg);
}

void mqtt_publish(void) {
    mqtt_publish_msg_t publish_msg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    mqtt_client_publish(&client, &publish_msg);
}

int main(void) {
    puts("MQTT client starting");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
