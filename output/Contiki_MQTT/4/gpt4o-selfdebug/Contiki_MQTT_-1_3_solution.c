#include <string.h>
#include <stdio.h>
#include "mqtt.h"
#include "process.h"
#include "netstack.h"
#include "timer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static struct mqtt_connection mqtt_conn;
static struct etimer periodic_timer;
static char pub_message[2][14] = {"work", "not work"};

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

PROCESS_THREAD(nrf52840_mqtt_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&mqtt_conn, &nrf52840_mqtt_process, "nrf52840_client", NULL, MQTT_BUF_SIZE);

    mqtt_connect(&mqtt_conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, CLOCK_SECOND * PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        static uint8_t counter = 0;  // Toggling work state
        char *message = pub_message[counter % 2];

        mqtt_publish(&mqtt_conn, NULL, "state/topic", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0);

        etimer_reset(&periodic_timer); // Reset timer for the next publication
        counter++;
    }

    PROCESS_END();
}