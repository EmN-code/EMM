#include <stdio.h>
#include <stdlib.h>
#include "net/rime/rime.h"
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"
#define PUBLISH_TOPIC "device/status"
#define RETRY_FOREVER -1

static char *pub_state = "work";
static struct etimer publish_timer;
static mqtt_client_t mqtt_client;
static struct mqtt_message mqtt_msg;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

void mqtt_event_cb(mqtt_client_t *client, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            etimer_set(&publish_timer, CLOCK_SECOND * 5);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH_CONFIRMED:
            printf("Message published successfully\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();
    
    printf("Starting MQTT Publish Process\n");

    mqtt_register(&mqtt_client, CLIENT_ID, mqtt_event_cb, MAX_TCP_SEGMENT_SIZE);

    mqtt_connect(&mqtt_client, BROKER_IP, BROKER_PORT, RETRY_FOREVER);

    while(1) {
        PROCESS_YIELD();
        if(ev == PROCESS_EVENT_TIMER) {
            mqtt_msg.payload = (uint8_t *)pub_state;
            mqtt_msg.payload_length = strlen(pub_state);
            mqtt_publish(&mqtt_client, NULL, PUBLISH_TOPIC, &mqtt_msg);
            
            etimer_reset(&publish_timer);
        }
    }
    
    PROCESS_END();
}

int main() {
    process_start(&mqtt_publish_process, NULL);
    return 0;
}