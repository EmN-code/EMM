#include "contiki.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "stdio.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL CLOCK_SECOND * 5
#define CLIENT_ID "nrf52840"
#define MQTT_TOPIC "device/status"

static struct etimer publish_timer;
static struct mqtt_connection conn;

PROCESS(mqtt_publish_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure and initiate the MQTT connection
    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL / CLOCK_SECOND * 3);

    // Wait for an acknowledgment that the connection is established
    while(conn.state != MQTT_CONN_STATE_CONNECTED) {
        PROCESS_PAUSE();
    }

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        if(conn.state == MQTT_CONN_STATE_CONNECTED) {
            const char *state = (CLOCK_SECOND % 10 > 5) ? "work" : "not work";
            mqtt_publish(&conn, NULL, MQTT_TOPIC, state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            printf("Published state: %s\n", state);
        } else {
            printf("MQTT disconnected, attempting to reconnect\n");
            mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL / CLOCK_SECOND * 3);
        }

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}