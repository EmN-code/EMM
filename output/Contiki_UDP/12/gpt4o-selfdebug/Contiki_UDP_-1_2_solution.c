#include <net/netstack.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/uip.h>
#include <net/ipv6/tcpip.h>
#include <stdio.h>

#define UDP_SERVER_ADDR   "47.102.103.1"
#define UDP_SERVER_PORT   20001
#define CLIENT_PORT       1234
#define SEND_INTERVAL     (5 * CLOCK_SECOND)  // 5 seconds send interval

static struct uip_udp_conn *client_conn;
static char state_message[] = "work or not work";

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  // Set up the UDP connection
  uip_ipaddr(&dest_ipaddr, 47, 102, 103, 1);
  client_conn = udp_new(&dest_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(CLIENT_PORT));

  etimer_set(&periodic_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&periodic_timer)) {
       uip_udp_packet_send(client_conn, state_message, sizeof(state_message));
       etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}