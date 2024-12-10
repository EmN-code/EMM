#include <string.h>
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "mqtt.h"
#include "net/ip/uip.h"
#include "simple-udp.h"
#include "mqtt-subscriber.h"
#include "contiki.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define STATE_TOPIC "device/state"
#define CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static char mqtt_client_id[20];
static struct etimer publish_timer;

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    printf("MQTT event received: %d\n", event);
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Publish event within MQTT\n");
            break;
        default:
            printf("Unknown MQTT event\n");
            break;
    }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    sprintf(mqtt_client_id, "%s", CLIENT_ID);

    mqtt_register(&conn, &mqtt_example_process, mqtt_client_id, mqtt_event_handler, 128);
    mqtt_set_username_password(&conn, "username", "password");

    uip_ipaddr_t broker_ipaddr;

    if(!uiplib_ipaddrconv(MQTT_BROKER_IP, &broker_ipaddr)) {
        printf("Failed to convert broker IP address\n");
        PROCESS_EXIT();
    }

    mqtt_connect(&conn, &broker_ipaddr, MQTT_BROKER_PORT, 100);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();
        if(ev == PROCESS_EVENT_TIMER && etimer_expired(&publish_timer)) {
            const char *message = "work";  // or "not work" based on actual state
            mqtt_publish(&conn, NULL, STATE_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}
