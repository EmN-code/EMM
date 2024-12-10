
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
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "riot_esp32"

#define INTERVAL_BETWEEN_PUBLISH (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(void);
static void mqtt_publish(void);

int main(void)
{
    puts("RIOT MQTT example");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL_BETWEEN_PUBLISH);
    }

    return 0;
}

static void mqtt_connect(void)
{
    mqtt_client_t client;
    mqtt_connect_info_t conn_info;

    memset(&client, 0, sizeof(client));
    memset(&conn_info, 0, sizeof(conn_info));

    conn_info.client_id = MQTT_CLIENT_ID;
    conn_info.keep_alive = MQTT_KEEPALIVE;
    conn_info.clean_session = MQTT_CLEAN_SESSION;

    mqtt_client_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);
}

static void mqtt_publish(void)
{
    mqtt_client_t client;
    mqtt_publish_info_t pub_info;

    memset(&client, 0, sizeof(client));
    memset(&pub_info, 0, sizeof(pub_info));

    pub_info.topic = MQTT_TOPIC;
    pub_info.payload = MQTT_MESSAGE;
    pub_info.payload_len = strlen(MQTT_MESSAGE);
    pub_info.qos = MQTT_QOS;
    pub_info.retain = MQTT_RETAIN;

    mqtt_client_publish(&client, &pub_info);
}
