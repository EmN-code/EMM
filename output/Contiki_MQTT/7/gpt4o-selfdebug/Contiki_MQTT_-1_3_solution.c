#include sys/process.h
#include <string.h>
#include <stdint.h>
#include "mqtt.h"

/* Declare the MAX_TCP_SEGMENT_SIZE that was previously undeclared */
#define MAX_TCP_SEGMENT_SIZE 32

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;

/* Callback for MQTT events */
void mqtt_event_callback(mqtt_status_t status)
{
    /* Handle MQTT events here if needed */
}

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static struct etimer publish_timer;
    static char mqtt_client_id[] = "contiki_mqtt_client";
    static char topic[] = "state";
    static char msg_work[] = "work";
    static char msg_not_work[] = "not work";
    static int work_state = 0;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, mqtt_client_id, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, "47.102.103.1", 1883, 60);

    while (1) {
        etimer_set(&publish_timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, topic, (uint8_t *) (work_state ? msg_work : msg_not_work), strlen(work_state ? msg_work : msg_not_work), 0);
        
        /* Toggle work state */
        work_state = !work_state;
    }

    PROCESS_END();
}