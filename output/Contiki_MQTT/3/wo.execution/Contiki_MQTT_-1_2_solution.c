#include <stdio.h>
#include "net/ipv6/uip-nameserver.h"
#include "contiki-default-conf.h"
#include "clock.h"
#include "net/ipv6/psock.h"
#include "sys/clock.h"
#include "contiki.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "net/mac/framer/framer.h"
#include "udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "coap-blocking-api.h"
#include "tcp-socket.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define CLIENT_ID "nrf52840-client"
#define TOPIC "device/status"

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char *state_message = "work or not work";

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event_handler, 44);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 100, 1, NULL);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        
        mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, 0, 0, NULL);

        etimer_reset(&publish_timer);
    }

    mqtt_disconnect(&conn, NULL);
    PROCESS_END();
}

void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
    switch (event) {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT broker\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("Disconnected from the MQTT broker - error: %d\n", ((mqtt_connack_event *)data)->return_code);
        break;
    default:
        break;
    }
}