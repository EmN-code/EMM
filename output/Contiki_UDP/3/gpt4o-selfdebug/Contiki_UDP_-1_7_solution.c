#include "net/ipv6/uip.h"
#include "contiki.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *client_conn;

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer periodic_timer;
  static char state_message[] = "work";

  PROCESS_BEGIN();

  // Set up a UDP connection to the server
  uip_ip6addr_t server_ipaddr;
  uip_ip6addr(&server_ipaddr, 0x2f2f, 0x2606, 0x4701, 0x1033, 0x0000, 0x0000, 0x0000, 0x0001);
  client_conn = udp_new(&server_ipaddr, UIP_HTONS(SERVER_PORT), NULL);

  if (!client_conn) {
    printf("Error: UDP connection could not be created\n");
    PROCESS_EXIT();
  }

  udp_bind(client_conn, UIP_HTONS(UIP_HTONS(SERVER_PORT)));

  // Set up a periodic timer to send messages every 5 seconds
  etimer_set(&periodic_timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    printf("Sending message to server: %s\n", state_message);
    uip_udp_packet_send(client_conn, state_message, sizeof(state_message));

    // Reset the timer to send the next message
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
