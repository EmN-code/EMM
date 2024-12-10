#include "simple-udp.h"
#include "mqtt.h"
#include "clock.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"
#define STATE_TOPIC "device/nrf52840/state"

static struct mqtt_connection conn;
static struct etimer timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void connack_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to the MQTT broker\n");
    } else {
        printf("Disconnected from the MQTT broker - error: %d\n", ((mqtt_connack_event_t *)data)->return_code);
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, connack_handler);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 100); // Keep alive is 100 seconds
    
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(mqtt_connected(&conn)) {
            char *state = "work"; // Could be conditionally changed
            mqtt_publish(&conn, NULL, STATE_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            printf("Published state: %s\n", state);
        } else {
            printf("Not connected to MQTT broker\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}