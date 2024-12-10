
#include <stdio.h>
#include <string.h>
#include "sys/etimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "contiki-conf.h"
#include "dev/leds.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

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
    // Handle incoming UDP packets if needed
}

void publish_state(void)
{
    static int state = 0;
    char message[20];
    uip_ipaddr_t broker_addr;

    // Toggle state
    state = !state;
    snprintf(message, sizeof(message), "State: %s", state ? "work" : "not work");

    // Convert IP address string to uip_ipaddr_t
    uip_ipaddr(&broker_addr, MQTT_BROKER_IP);

    // Send the message to the MQTT broker
    simple_udp_sendto(&udp_conn, message, strlen(message), &broker_addr);
}

int main(void)
{
    // Initialize the Contiki system
    process_init();
    process_start(&etimer_process, NULL);
    leds_init();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, MQTT_BROKER_PORT, udp_rx_callback);

    // Set up the periodic timer
    etimer_set(&et, PUBLISH_INTERVAL);

    while(1) {
        // Run the Contiki process
        process_run();

        // Check if the timer has expired
        if(etimer_expired(&et)) {
            publish_state();
            etimer_reset(&et);
        }
    }

    return 0;
}
