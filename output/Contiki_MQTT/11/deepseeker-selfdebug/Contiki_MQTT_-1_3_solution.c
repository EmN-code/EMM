
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/netif.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-prefix.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-prefix.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-prefix.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-prefix.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-prefix.h"
#include "net/ipvtt.h"
#include "net/mqtt-transport.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static uint16_t mid;
    static char payload[20];

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 0);
    mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 0, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        strcpy(payload, "work");
        mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_0, 0, 0, 0, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&mqtt_publish_process, NULL);
    return 0;
}
