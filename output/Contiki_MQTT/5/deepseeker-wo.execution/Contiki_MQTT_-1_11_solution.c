
#include "contiki-default-conf.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "string.h"
#include "stdio.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS_LEVEL 0

static struct mqtt_connection conn;
static struct etimer et;

void event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, NULL, "nrf52840_client", event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS_LEVEL, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
