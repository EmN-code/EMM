#include "net/netstack.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include <stdlib.h>
#include <string.h>
#include "contiki.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct udp_socket mqtt_socket;
static uip_ipaddr_t broker_ipaddr;
static char payload[50];
static int publish_state = 0;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Set UDP connection
    uip_ipaddr(&broker_ipaddr, 47,102,103,1);
    udp_socket_register(&mqtt_socket, NULL, NULL);
    udp_socket_bind(&mqtt_socket, 0);
    udp_socket_connect(&mqtt_socket, &broker_ipaddr, MQTT_BROKER_PORT);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();
        if (etimer_expired(&timer)) {
            // Prepare the payload
            snprintf(payload, sizeof(payload), "State: %s", (publish_state % 2 == 0) ? "work" : "not work");

            // Publish message
            uip_udp_packet_send(&mqtt_socket, payload, strlen(payload));

            // Toggle state
            publish_state++;

            // Reset the timer
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}