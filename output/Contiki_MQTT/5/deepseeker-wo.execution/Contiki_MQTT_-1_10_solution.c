
#include "contiki-default-conf.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/clock.h"
#include "string.h"
#include "random.h"
#include "stdio.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"

static struct mqtt_connection conn;
static struct etimer et;

void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(void) {
    static int state = 0;
    const char *payload = state ? MQTT_PAYLOAD_WORK : MQTT_PAYLOAD_NOT_WORK;
    state = !state;

    if (mqtt_connected(&conn)) {
        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, NULL);
    }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
        publish_state();
    }

    PROCESS_END();
}
