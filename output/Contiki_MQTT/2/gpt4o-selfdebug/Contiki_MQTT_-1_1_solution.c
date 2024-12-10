#include "netstack_ip_action.h"
#include "mqtt_prop_encode.h"
#include "mqtt_connection.h"
#include "etimer.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static struct etimer timer;

static void prepare_mqtt_connection(void) {
    mqtt_connect(&conn, "47.102.103.1", 1883, 100);
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "pub_client",
                  mqtt_event, MAX_TCP_SEGMENT_SIZE);

    etimer_set(&timer, CLOCK_SECOND * 5);

    prepare_mqtt_connection();

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(conn.status == MQTT_STATUS_CONNECTED) {
            char payload[20];
            strcpy(payload, "work");
            mqtt_publish(&conn, NULL, "state", (uint8_t *)payload,
                         strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            printf("Published: %s\n", payload);
        } else {
            printf("Trying to reconnect...\n");
            prepare_mqtt_connection();
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event,
                       void *data) {
    switch(event) {
    case MQTT_EVENT_CONNECTED:
        printf("MQTT Connected\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT Disconnected\n");
        break;
    case MQTT_EVENT_PUBLISH:
        printf("Got a publish event\n");
        break;
    case MQTT_EVENT_SUBACK:
        printf("Subscribe acknowledged\n");
        break;
    default:
        printf("Other event: %d\n", event);
        break;
    }
}