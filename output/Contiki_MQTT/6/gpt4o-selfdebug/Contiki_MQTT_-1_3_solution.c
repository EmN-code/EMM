#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uiplib.h"

#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

static struct mqtt_connection conn;
static char state_message[16];

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, "client_id",
                  NULL, 64);
    mqtt_connect(&conn, "47.102.103.1", 1883, 100);

    while (1) {
        etimer_set(&timer, 5 * CLOCK_SECOND);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        snprintf(state_message, sizeof(state_message), "work");

        mqtt_publish(&conn, NULL, "state_topic",
                     (uint8_t *) state_message,
                     strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    }

    PROCESS_END();
}