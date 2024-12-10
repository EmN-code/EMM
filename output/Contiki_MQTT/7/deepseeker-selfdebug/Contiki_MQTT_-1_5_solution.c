
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ip/simple-udp.h"
#include "lib/random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/log.h"

#include <stdio.h>
#include <string.h>

#define LOG_MODULE "MQTT Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection mqtt_conn;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static void
mqtt_send_state(void)
{
    static char buf[128];
    int len;

    len = snprintf(buf, sizeof(buf), "%s", MQTT_MESSAGE);
    if(len > 0) {
        simple_udp_sendto(&mqtt_conn, buf, len, uip_ipaddr(MQTT_BROKER_IP), MQTT_BROKER_PORT);
        LOG_INFO("Sent state: %s\n", MQTT_MESSAGE);
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    static struct etimer et;

    PROCESS_BEGIN();

    simple_udp_register(&mqtt_conn, 0, NULL, MQTT_BROKER_PORT, NULL);

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_send_state();

        etimer_reset(&et);
    }

    PROCESS_END();
}
