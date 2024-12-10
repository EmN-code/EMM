#include "simple-udp.h"
#include "net/ip/simple-udp-message.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "contiki.h"
#include <stdio.h>

#define UDP_PORT 20001
#define SERVER_IP_ADDR "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer send_timer;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  // Set up the UDP connection
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  // Convert server IP address
  uiplib_ipaddrconv(SERVER_IP_ADDR, &server_ipaddr);

  // Set timer for periodic sends
  etimer_set(&send_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));

    const char *device_state = "work";  // Example device state, should be fetched dynamically
    printf("Sending device state: %s\n", device_state);
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), &server_ipaddr);

    // Reset the timer
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}