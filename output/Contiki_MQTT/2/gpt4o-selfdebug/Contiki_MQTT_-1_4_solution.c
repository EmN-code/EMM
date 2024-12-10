#include "net/ipv6/simple-udp.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "sys/process.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "mqtt-srv.h"
#include <stdio.h>
#include <string.h>

// Define the connection parameters
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_TOPIC "nrf52840/state"
#define CLIENT_ID "nrf52840"
#define STATE_MSG "work or not work"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    static struct etimer timer;
    static mqtt_status_t status;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, mqtt_event_callback, 256);
    status = mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    if (status != MQTT_STATUS_OK) {
        printf("Failed to connect to the MQTT broker: %d\n", status);
        PROCESS_EXIT();
    }

    etimer_set(&timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        mqtt_publish(&conn, NULL, PUB_TOPIC, (uint8_t *)STATE_MSG, strlen(STATE_MSG), 
                     MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published: %s\n", STATE_MSG);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
