#include "contiki.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/netstack.h"  

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct etimer periodic_timer;
static struct uip_udp_conn *client_conn;

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS_THREAD(udp_client_process, ev, data){
  static char msg[20];
  static int count = 0;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  etimer_set(&periodic_timer, SEND_INTERVAL);

  uip_ip6addr(&server_ipaddr, 
    UIP_HTONL(0x20010db8), 0, 0, 0, UIP_HTONL(0x00004766), UIP_HTONL(0x00000100), 0, 0);

  client_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(UDP_SERVER_PORT));

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if(count % 2 == 0) {
      snprintf(msg, sizeof(msg), "Device: working");
    } else {
      snprintf(msg, sizeof(msg), "Device: not working");
    }

    uip_udp_packet_sendto(client_conn, msg, strlen(msg),
                          &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

    etimer_reset(&periodic_timer);
    count++;
  }

  PROCESS_END();
}