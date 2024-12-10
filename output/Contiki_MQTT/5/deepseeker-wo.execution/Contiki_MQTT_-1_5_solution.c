
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS MQTT_QOS_LEVEL_0

static struct mqtt_connection conn;
static uint16_t mid;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        LOG_INFO("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        LOG_INFO("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    static struct etimer et;

    PROCESS_BEGIN();

    LOG_INFO("Starting MQTT example\n");

    mqtt_register(&conn, PROCESS_CURRENT(), "client_id", event_callback, 128);

    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS, 0, 0, 0, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
