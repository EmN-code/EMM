# Include necessary header files
#include "sys/etimer.h"
#include "simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ip/uip.h"
#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;
#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"
static char device_state[] = "work or not work";

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer timer;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  // Initialize the UDP connection
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  // Set the server IP address
  uip_ip6addr(&server_ipaddr, 0, 0, 0, 0, 0, 0, 0xffff, UIP_HTONL(((47 << 24) | (102 << 16) | (103 << 8) | 1)));

  etimer_set(&timer, 5 * CLOCK_SECOND);
  while (1) {
    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Send the device state to the server
    simple_udp_send(&udp_conn, device_state, strlen(device_state));

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}