#include "etimer.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "lib/random.h"
#include "os/services/mqtt/mqtt.h"
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_demo_process, "MQTT Demo Process");
AUTOSTART_PROCESSES(&mqtt_demo_process);

static struct mqtt_connection conn;
static struct etimer periodic_timer;
static char pub_topic[] = "device/status";
static char pub_message[] = "work";

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT Connected to broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT Disconnected from broker\n");
            break;
        case MQTT_EVENT_PUBLISH: {
            struct mqtt_puback_event *puback_event = (struct mqtt_puback_event *)data;
            printf("Published message to topic %s\n", puback_event->topic);
            break;
        }
        default:
            printf("MQTT Event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_demo_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_demo_process, MQTT_BROKER_IP, mqtt_event, sizeof(struct mqtt_connection));
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, CLOCK_SECOND * 10);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, 0);
        printf("Publishing: %s to topic: %s\n", pub_message, pub_topic);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
