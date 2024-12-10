
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <mqtt.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct sock_tcp_socket sock;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(struct mqtt_connection *conn, const char *state) {
    mqtt_publish(conn, MQTT_TOPIC, state, strlen(state), MQTT_QOS_0, false);
}

int main(void) {
    printf("Starting MQTT client\n");

    // Initialize the MQTT connection
    mqtt_init(&conn, &sock);

    // Register the MQTT client
    mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_callback, 0);

    // Connect to the MQTT broker
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, false, NULL);

    // Publish state every 5 seconds
    while (1) {
        publish_state(&conn, "work");
        xtimer_sleep(5);
        publish_state(&conn, "not work");
        xtimer_sleep(5);
    }

    return 0;
}
