#include <stdio.h>
#include <stdint.h>
#include "simple-udp.h"
#include "net/ip/uip.h"
#include "contiki.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND) // Send every 5 seconds

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer et;
  static char device_state[16];
  uip_ipaddr_t server_ipaddr;
  
  PROCESS_BEGIN();

  // Set the device state, here we assume "work" or "not work"
  snprintf(device_state, sizeof(device_state), "work"); // You can modify this as needed

  // Set server IP address
  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);

  // Initialize UDP connection
  simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

  while(1) {
    // Set timer for sending intervals
    etimer_set(&et, SEND_INTERVAL);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send the device state to the server
    simple_udp_send(&udp_conn, device_state, strlen(device_state) + 1);

    printf("Sent device state: %s\n", device_state);
  }

  PROCESS_END();
}