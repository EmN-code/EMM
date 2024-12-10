
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_publish(void)
{
    static uint8_t buf[128];
    static mqtt_connection_t conn;
    static mqtt_message_t msg;

    memset(&conn, 0, sizeof(conn));
    memset(&msg, 0, sizeof(msg));

    conn.host = MQTT_BROKER_IP;
    conn.port = MQTT_BROKER_PORT;
    conn.client_id = "riot_client";
    conn.keep_alive = MQTT_KEEPALIVE;
    conn.username = MQTT_USERNAME;
    conn.password = MQTT_PASSWORD;

    msg.topic = MQTT_TOPIC;
    msg.payload = MQTT_MESSAGE;
    msg.payload_len = strlen(MQTT_MESSAGE);
    msg.qos = MQTT_QOS;
    msg.retain = MQTT_RETAIN;
    msg.dup = MQTT_DUP;

    mqtt_connect(&conn);
    mqtt_publish(&conn, &msg);
    mqtt_disconnect(&conn);
}

int main(void)
{
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
