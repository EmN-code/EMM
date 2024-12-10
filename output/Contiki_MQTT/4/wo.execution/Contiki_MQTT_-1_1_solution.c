#include <stdio.h>
#include "contiki.h"
#include "mqtt.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/etimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID "nrf52840_mqtt"
#define MQTT_TOPIC "device/state"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            etimer_set(&publish_timer, PUBLISH_INTERVAL);
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

    uip_ipaddr_t broker_addr;
    mqtt_register(&conn, &mqtt_example_process, MQTT_CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);
    uip_ip6addr(&broker_addr, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 0);
    mqtt_connect(&conn, &broker_addr, MQTT_BROKER_PORT);

    while(1) {
        PROCESS_YIELD();
        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            const char *pub_message = "work or not work";
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}