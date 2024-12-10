
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct sock_tcp_connection tcp_conn;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

static void publish_state(void) {
    mqtt_publish(&conn, MQTT_TOPIC, MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS, MQTT_RETAIN);
}

int main(void) {
    printf("Starting MQTT client\n");

    /* Initialize the MQTT connection */
    mqtt_init(&conn, &tcp_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID, mqtt_event_callback, NULL);

    /* Connect to the MQTT broker */
    if (mqtt_connect(&conn, MQTT_KEEPALIVE) != MQTT_OK) {
        printf("MQTT connection failed\n");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
