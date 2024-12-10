#include "simple_udp.h"
#include "mqtt_connection.h"
#include "etimer.h"
#include "mqtt.h"
#include <stdio.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL 5
#define PUBLISH_TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer publish_timer;

void publish_state() {
    char *state = "work"; // Example state, can be "work" or "not work"
    mqtt_publish(&conn, PUBLISH_TOPIC, state, strlen(state), 1, 0);
    printf("Published state: %s\n", state);
}

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Publish event\n");
            break;
        default:
            break;
    }
}

int main(void) {
    mqtt_register(&conn, NULL, MQTT_SERVER_IP, PUBLISH_INTERVAL, mqtt_event_callback);
    mqtt_connect(&conn, MQTT_SERVER_IP, MQTT_SERVER_PORT, PUBLISH_INTERVAL);

    etimer_set(&publish_timer, CLOCK_SECOND * PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        publish_state();
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
