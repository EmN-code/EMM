#include "simple_udp.h"
#include "netstack.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_connection conn;
static char *topics[] = {"state"};
static char client_id[] = "nrf52840-client";

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

// Function to handle MQTT events
void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT Connected\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT Disconnected\n");
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer periodic_timer;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, client_id, mqtt_event_callback, MQTT_MAX_PACKET_SIZE);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();
        
        if(etimer_expired(&periodic_timer)) {
            // Publish 'work' state
            static uint16_t packet_id;
            const char* state = "work";
            mqtt_publish(&conn, &packet_id, topics[0], (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0);
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}
