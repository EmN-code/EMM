#include "os/net/netstack.h"
#include "os/sys/etimer.h"
#include "os/sys/ctimer.h"
#include "os/services/mqtt/mqtt.h"
#include <stdio.h>

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer timer;
static char state_message[50];

void mqtt_event_callback(mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    // Configure MQTT connection
    static mqtt_connection_t conn;
    mqtt_register(&conn, &mqtt_example_process, "mqtt_client",
                  mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, "47.102.103.1", 1883,
                 MQTT_KEEPALIVE, MQTT_CLEAN_SESSION_ON);

    // Set an etimer to go off in 5 seconds and repeat
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&timer)) {
            snprintf(state_message, sizeof(state_message), "state: %s", "work");
            mqtt_publish(&conn, NULL, "state/topic", (uint8_t *)state_message,
                         strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&timer);  // Reset the timer to go off again after 5 seconds
        }
    }

    PROCESS_END();
}
