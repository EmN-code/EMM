#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip.h"
#include "net/ip/uip.h"
#include "net/netstack.h"
#include "contiki-default-conf.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;
static char client_id[20];
static char pub_msg[20];

void publish_state(void) {
    static uint8_t state = 0;
    snprintf(pub_msg, sizeof(pub_msg), "state: %s", state == 0 ? "work" : "not work");
    state = !state;
    mqtt_publish(&conn, NULL, "device/state", (uint8_t *)pub_msg, strlen(pub_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();
    snprintf(client_id, sizeof(client_id), "nrf52840_%02x%02x", linkaddr_node_addr.u8[6], linkaddr_node_addr.u8[7]);
    mqtt_register(&conn, &mqtt_client_process, client_id, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUB_INTERVAL);

    etimer_set(&periodic_timer, PUB_INTERVAL);
    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&periodic_timer)) {
            publish_state();
            etimer_reset(&periodic_timer);
        }
    }
    PROCESS_END();
}

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}