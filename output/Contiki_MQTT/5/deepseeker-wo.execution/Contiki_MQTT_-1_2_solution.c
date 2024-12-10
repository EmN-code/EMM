
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"

#include "mqtt.h"
#include "mqtt-client.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection mqtt_conn;
static struct etimer et;

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static char message[32];
    static int state = 0;

    PROCESS_BEGIN();

    mqtt_init(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, mqtt_event_handler, NULL);
    mqtt_connect(&mqtt_conn);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (state == 0) {
            strcpy(message, "work");
            state = 1;
        } else {
            strcpy(message, "not work");
            state = 0;
        }

        mqtt_publish(&mqtt_conn, MQTT_TOPIC, message, strlen(message), 0, 0);

        etimer_reset(&et);
    }

    PROCESS_END();
}
