#include <stdio.h>
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "mqtt_status_codes.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"
#define PUB_TOPIC "device/state"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_PUBLISH) {
        printf("Message published successfully\n");
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event, 256);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL, MQTT_CLEAN_SESSION_ON);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&publish_timer)) {
            char payload[] = "work";
            uint16_t mid;
            mqtt_publish(&conn, &mid, PUB_TOPIC, (uint8_t *)payload, sizeof(payload), MQTT_QOS_LEVEL_0);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}
