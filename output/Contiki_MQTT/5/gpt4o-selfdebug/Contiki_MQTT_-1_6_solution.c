#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "contiki.h"
#include "contiki-net.h"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define TOPIC "device/status"
#define PUBLISH_PAYLOAD "work or not work"

PROCESS(nrf52840_mqtt_publisher, "NRF52840 MQTT Publisher");
AUTOSTART_PROCESSES(&nrf52840_mqtt_publisher);

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to broker\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("Disconnected from broker\n");
        break;
    case MQTT_EVENT_PUBLISH://
        printf("Published successfully\n");
        break;
    case MQTT_EVENT_SUBACK:
    case MQTT_EVENT_UNSUBACK:
    case MQTT_EVENT_PUBACK:
        break;
    default:
        printf("Unhandled MQTT event: %d\n", event);
        break;
    }
}

PROCESS_THREAD(nrf52840_mqtt_publisher, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &nrf52840_mqtt_publisher, "nrf52840", mqtt_event_handler, 256);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 100);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER) {
            mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)PUBLISH_PAYLOAD,
                strlen(PUBLISH_PAYLOAD), MQTT_QOS_LEVEL_0, 0); // Ensure correct pointer sign
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}