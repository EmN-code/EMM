#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcp-socket.h"
#include "mqtt.h"
#include "contiki.h"
#include <string.h>

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char *broker_ip = "47.102.103.1";
static uint16_t broker_port = 1883;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connection established\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT connection terminated\n");
            break;
        default:
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, "client_id", mqtt_event_callback, 256);
    mqtt_connect(&conn, broker_ip, broker_port, 60, 1);

    while (1) {
        etimer_set(&publish_timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        static char *state = "work";
        mqtt_publish(&conn, NULL, "state/topic", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0);
        printf("Published state: %s\n", state);
    }

    PROCESS_END();
}