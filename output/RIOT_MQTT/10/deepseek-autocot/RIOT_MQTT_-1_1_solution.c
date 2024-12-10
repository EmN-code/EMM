
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

static sock_udp_ep_t broker = { .port = MQTT_BROKER_PORT, .family = AF_INET };
static mqtt_client_t client;
static uint8_t sendbuf[1024];
static uint8_t readbuf[1024];

void mqtt_connect(void) {
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    int res = mqtt_connect(&client, &broker, &conn_info, NULL, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    if (res != MQTT_OK) {
        printf("MQTT connection failed: %d\n", res);
    } else {
        printf("MQTT connected to broker\n");
    }
}

void mqtt_publish(void) {
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    int res = mqtt_publish(&client, &pub_info, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    if (res != MQTT_OK) {
        printf("MQTT publish failed: %d\n", res);
    } else {
        printf("MQTT message published\n");
    }
}

int main(void) {
    // Initialize network stack
    sock_udp_ep_t local = { .port = 0, .family = AF_INET };
    sock_udp_t sock;
    sock_udp_create(&sock, &local, &broker, 0);

    // Initialize MQTT client
    mqtt_client_init(&client, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
