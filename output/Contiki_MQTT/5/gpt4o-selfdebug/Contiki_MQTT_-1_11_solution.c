#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#include "contiki.h"
#include "mqtt.h"

#include <stdio.h>
#include <string.h>

#define MAX_TCP_SEGMENT_SIZE 32
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char broker_address[] = "47.102.103.1";

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static struct etimer timer;
    static char pub_topic[] = "state";
    static char buf[100];
    static uint16_t packet_id = 0;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_client",
                  mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);

    mqtt_connect(&conn, broker_address, 1883, 100);

    if (!mqtt_connected(&conn)) {
        printf("Failed to connect to MQTT broker\n");
        PROCESS_EXIT();
    }

    etimer_set(&timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        snprintf(buf, sizeof(buf), "work");

        mqtt_publish(&conn, &packet_id, pub_topic, (uint8_t *)buf, strlen(buf),
                     MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, false, NULL);

        printf("State published: %s\n", buf);

        etimer_reset(&timer);
    }

    PROCESS_END();
}

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event,
                         void *data) {
    switch(event) {
    case MQTT_EVENT_CONNECTED:
        printf("MQTT connection established\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT connection disconnected\n");
        break;
    case MQTT_EVENT_PUBLISH:
        printf("MQTT message published\n");
        break;
    default:
        break;
    }
}