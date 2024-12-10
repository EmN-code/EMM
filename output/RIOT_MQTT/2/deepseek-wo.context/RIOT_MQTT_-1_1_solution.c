
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(void)
{
    static int state = 0;
    const char *message = (state == 0) ? "work" : "not work";
    state = 1 - state; // Toggle state

    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    mqtt_message_t msg = {
        .topic = MQTT_TOPIC,
        .payload = (void *)message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    sock_udp_ep_t broker = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT,
    };

    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    if (mqtt_connect(&client, &broker, &conn_info) == 0) {
        mqtt_publish(&client, &msg);
        mqtt_disconnect(&client);
    }
}

int main(void)
{
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
