#include "contiki.h"
#include "os/dev/leds.h"
#include "simple-udp.h"
#include "contiki-net.h"
#include "random.h"
#include "mqtt.h"
#include "stdio.h"
#include "string.h"

static char broker_ip[] = "47.102.103.1";
static uint16_t broker_port = 1883;
static char pub_topic[] = "device/state";
static char client_id[] = "nrf52840_client";

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer timer;
static struct mqtt_connection conn;
static char app_state[10] = "work";

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Published message\n");
            break;
        default:
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_example_process, client_id, mqtt_event, 256);
    mqtt_connect(&conn, broker_ip, broker_port, 100);
    
    etimer_set(&timer, 5 * CLOCK_SECOND);
    
    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&timer)) {
            mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)app_state, strlen(app_state), 0, 0);
            printf("State published: %s\n", app_state);

            // Flip state between "work" and "not work"
            if(strcmp(app_state, "work") == 0) {
                strcpy(app_state, "not work");
            } else {
                strcpy(app_state, "work");
            }

            etimer_reset(&timer);
        }
    }
    
    PROCESS_END();
}