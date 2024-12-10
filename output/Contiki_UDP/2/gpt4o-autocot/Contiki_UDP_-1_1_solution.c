#include <stdio.h>
#include "net/simple-udp.h"
#include "net/netstack.h"
#include "contiki.h"

#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static char device_state[10];

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  etimer_set(&periodic_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);

    // Set the device state
    snprintf(device_state, sizeof(device_state), "work");  // Adjust this as necessary

    // Send the device state to the UDP server
    printf("Sending device state: %s\n", device_state);
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state) + 1,
                      &linkaddr_node_addr(SERVER_IP));
  }

  PROCESS_END();
}
