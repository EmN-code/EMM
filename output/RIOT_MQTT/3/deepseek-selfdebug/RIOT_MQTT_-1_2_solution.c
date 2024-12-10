
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void publish_state(void) {
    static uint8_t buf[128];
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_client_init(&client);
    mqtt_client_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);

    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, sizeof(MQTT_MESSAGE) - 1, 0, 0);

    mqtt_client_disconnect(&client);
}

int main(void) {
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}
