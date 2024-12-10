
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static uint16_t mid;
    static char payload[10];

    PROCESS_BEGIN();

    memset(&conn, 0, sizeof(conn));
    mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 1024);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, NULL);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        strcpy(payload, "work");
        mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
