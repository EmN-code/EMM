
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
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "string.h"
#include "net/routing/routing.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "sys/log.h"
#include "stdio.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "stdio.h"
#include "contiki.h"
#include "net/netstack.h"
#include "sys/clock.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"

struct mqtt_connection conn;

void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static struct etimer et;
    static uint16_t mid;
    static char payload[10];

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", event_callback, 128);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            strcpy(payload, (random_rand() % 2 == 0) ? PAYLOAD_WORK : PAYLOAD_NOT_WORK);
            mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
