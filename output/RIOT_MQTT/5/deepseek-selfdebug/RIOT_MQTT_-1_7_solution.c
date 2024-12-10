
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_FLAG 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_KEEP_ALIVE 60
#define MQTT_TIMEOUT 10000

static void mqtt_connect(void)
{
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEP_ALIVE,
        .clean_session = MQTT_CLEAN_SESSION,
        .will_flag = MQTT_WILL_FLAG,
        .will = {
            .topic = NULL,
            .message = NULL,
            .qos = 0,
            .retain = 0
        },
        .username = NULL,
        .password = NULL
    };

    mqtt_client_init(&client);
    mqtt_client_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);
}

static void mqtt_publish(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    mqtt_client_publish(&client, &pub_info);
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
