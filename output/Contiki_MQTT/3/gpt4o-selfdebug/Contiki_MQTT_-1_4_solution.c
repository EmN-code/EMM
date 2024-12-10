#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#include <string.h>
#include "contiki.h"
#include "mqtt.h"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static char *broker_ip = "47.102.103.1";
static uint8_t payload[] = "work";
static char *topic = "device/status";

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        default:
            printf("Unhandled MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, "nrf52840_mqtt_client", mqtt_event_handler, 128);
    mqtt_connect(&conn, broker_ip, 1883, 100, 0);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &periodic_timer) {
            mqtt_publish(&conn, NULL, topic, payload, sizeof(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}