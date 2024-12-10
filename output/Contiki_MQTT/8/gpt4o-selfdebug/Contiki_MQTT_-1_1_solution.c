#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "net/netstack.h"
#include "os/sys/etimer.h"
#include "os/sys/clock.h"
#include "dev/leds.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct etimer publish_timer;
static struct mqtt_connection conn;

static void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        default:
            printf("MQTT event %d\n", event);
            break;
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, "client_id", mqtt_event_handler, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, (60 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        if (mqtt_connected(&conn)) {
            printf("Publishing state: work\n");
            mqtt_publish(&conn, NULL, "state", (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        }

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}