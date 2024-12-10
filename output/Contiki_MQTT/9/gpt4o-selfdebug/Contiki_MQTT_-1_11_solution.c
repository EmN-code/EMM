#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "mqtt.h"

#define BROKER_IP_ADDR  "47.102.103.1"
#define BROKER_PORT     1883
#define KEEP_ALIVE_TIME 60
#define CLIENT_ID       "nrf52840-client"
#define TOPIC           "state"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, NULL, MQTT_MAX_SEGMENT_SIZE);

    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, KEEP_ALIVE_TIME, 1, NULL);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            if(conn.state == MQTT_CONN_STATE_CONNECTED) {
                static uint16_t message_id;
                const char *message = "work";

                mqtt_publish(&conn, &message_id, TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
            }

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}