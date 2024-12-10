#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "mqtt.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define PUB_INTERVAL (5 * CLOCK_SECOND)
#define MAX_TCP_SEGMENT_SIZE 32

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static char pub_topic[] = "device/state";
static char pub_message[20];

static struct etimer periodic_timer;

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, MQTT_CLIENT_ID, NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 0);

    while(1) {
        snprintf(pub_message, sizeof(pub_message), "work");
        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_set(&periodic_timer, PUB_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    }

    PROCESS_END();
}
