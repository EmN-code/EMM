
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "dev/leds.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static struct etimer et;

void udp_rx_callback(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen)
{
    printf("Received response from MQTT broker\n");
}

void publish_state(void)
{
    static char message[64];
    snprintf(message, sizeof(message), "%s", MQTT_MESSAGE);

    uip_ipaddr_t broker_addr;
    uip_ipaddr(&broker_addr, MQTT_BROKER_IP);

    simple_udp_sendto(&udp_conn, message, strlen(message), &broker_addr);
    printf("Published state: %s\n", message);
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, MQTT_BROKER_PORT, udp_rx_callback);

    etimer_set(&et, MQTT_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        publish_state();

        etimer_reset(&et);
    }

    PROCESS_END();
}
