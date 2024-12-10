#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ip/uip-udp-packet.h"
#include <string.h>
#include <stdio.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *client_conn;
static char msg[20];

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  // Set a timer to trigger every 5 seconds
  etimer_set(&periodic_timer, SEND_INTERVAL);

  // Create a new UDP connection to the server
  uip_ipaddr_t ipaddr;
  uiplib_ipaddrconv(UDP_SERVER_IP, &ipaddr);
  client_conn = udp_new(&ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    snprintf(msg, sizeof(msg), "Device work state");
    printf("Sending message: %s\n", msg);
    uip_udp_packet_send(client_conn, msg, strlen(msg));

    // Reset the timer for the next interval
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}