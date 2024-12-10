#include "net/ipv6/uip-udp-packet.h"
#include "dev/dht11-sensor.h"
#include "mqtt.h"
#include "process.h"
#include "etimer.h"
#include <stdio.h>

PROCESS(mqtt_publisher_process, "MQTT Publisher Example");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

// MQTT configurations
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID "nrf52840-mqtt-client"
#define MQTT_PUBLISH_TOPIC "mqtt/nrf52840/status"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, MQTT_CLIENT_ID, NULL, 128);

    uip_ipaddr_t broker_ip;
    uip_ipaddr(&broker_ip, 47, 102, 103, 1);
    mqtt_connect(&conn, &broker_ip, BROKER_PORT, PUBLISH_INTERVAL * 3, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&periodic_timer)) {
            printf("Publishing state\n");
            mqtt_publish(&conn, NULL, MQTT_PUBLISH_TOPIC, (uint8_t *)"work", 4, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}