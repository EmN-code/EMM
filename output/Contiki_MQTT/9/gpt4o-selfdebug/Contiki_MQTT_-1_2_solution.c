#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "contiki.h"
#include <string.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

static struct mqtt_connection conn;
static char *state_message = "work or not work";

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void publish_state(void) {
    uint16_t mid;
    mqtt_publish(&conn, &mid, "sensor/state", (uint8_t *)state_message, strlen(state_message), MQTT_QOS_0, 0, 0, 0, NULL);
}

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 0, NULL);
    
    etimer_set(&timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        publish_state();
        etimer_reset(&timer);
    }

    PROCESS_END();
}