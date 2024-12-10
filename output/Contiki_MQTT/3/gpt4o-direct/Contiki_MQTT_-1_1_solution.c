#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/etimer.h"
#include "mqtt.h"
#include "net/ipv6/uip.h"
#include "contiki.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5 * CLOCK_SECOND)
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_TOPIC "device/state"

/*---------------------------------------------------------------------------*/
PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);
/*---------------------------------------------------------------------------*/
static struct etimer timer;
static struct mqtt_connection conn;
static char *states[] = {"work", "not work"};
static int state_index = 0;

/*---------------------------------------------------------------------------*/

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT Broker!\n");
    }
    else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT Broker.\n");
    }
}

/*---------------------------------------------------------------------------*/

static void publish_state(void) {
    if(mqtt_connected(&conn)) {
        char payload[20];
        snprintf(payload, sizeof(payload), "State: %s", states[state_index]);
        mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)payload, strlen(payload),
                     MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published: %s\n", payload);
        state_index = (state_index + 1) % 2;  // Toggle between states
    }
    else {
        printf("MQTT not connected. Attempting to reconnect...\n");
        mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                     60 * CLOCK_SECOND, CLIENT_ID, NULL, NULL, 0);
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, mqtt_event,
                  BUF_SIZE);
    etimer_set(&timer, PUB_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        publish_state();
        etimer_reset(&timer);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/