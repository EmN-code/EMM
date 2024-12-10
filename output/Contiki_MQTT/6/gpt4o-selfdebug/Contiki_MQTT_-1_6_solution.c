#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include <stdio.h>
#include <stdint.h>
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_status_t status;
    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    status = mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, NULL, MQTT_PUBLISH_INTERVAL);
    if (status != MQTT_STATUS_OK) {
        printf("Failed to register MQTT: %d\n", status);
    }
    mqtt_connect(&conn);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&publish_timer)) {
            printf("Publishing state to MQTT broker\n");
            mqtt_publish(&conn, NULL, "state", (uint8_t *)"work", 5, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}