#include "net/ipv6/uip-ds6-route.h"
#include "mqtt.h"
#include "net.h"
#include "contiki.h"
#include <stdio.h>
#include <stdint.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer pub_timer;
static struct mqtt_connection conn;
static char *topic = "state";
static uint8_t payload[] = "work";

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    printf("MQTT event received: %d\n", event);
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_example_process, "mqtt_client", mqtt_event_callback, NETSTACK_MAX_SEGMENT_SIZE);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, MQTT_PROP_LIST_NONE);

    etimer_set(&pub_timer, PUB_INTERVAL);

    while(1) {
        PROCESS_YIELD();
        
        if(ev == PROCESS_EVENT_TIMER && data == &pub_timer) {
            mqtt_publish(&conn, NULL, topic, payload, sizeof(payload), MQTT_QOS_LEVEL_0, MQTT_PROP_LIST_NONE);
            etimer_reset(&pub_timer);
        }
    }

    mqtt_disconnect(&conn, MQTT_PROP_LIST_NONE);

    PROCESS_END();
}