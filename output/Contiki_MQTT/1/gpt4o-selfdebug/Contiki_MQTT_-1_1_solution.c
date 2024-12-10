#include <string.h>
#include "net/netstack.h"
#include "os/sys/etimer.h"
#include "mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "device/state"

static struct etimer publish_timer;
static struct mqtt_connection conn;

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();
    mqtt_register(&conn, &mqtt_publish_process, MQTT_CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 100);

    etimer_set(&publish_timer, CLOCK_SECOND * PUBLISH_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        etimer_reset(&publish_timer);
         
        char *state = "work";
        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published: %s\n", state);
    }

    PROCESS_END();
}
