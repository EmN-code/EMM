#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>

#define MAX_TCP_SEGMENT_SIZE 32
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID "nrf52840-client"
#define MQTT_TOPIC "device/state"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    printf("MQTT event %d\n", event);
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, MQTT_CLIENT_ID, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    uip_ipaddr_t broker_ip;
    uip_ip6addr(&broker_ip, 0x47,0x102,0x103,0x0000,0x0000,0x0000,0x0000,0x0001); // 47.102.103.1 IPv6 form

    mqtt_connect(&conn, &broker_ip, MQTT_PORT, 1000, MQTT_CLEAN_SESSION_ON);

    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        printf("Publishing state\n");
        char message[] = "work";  // or "not work" depending on application logic
        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}