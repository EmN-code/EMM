#include <string.h>
#include <stdio.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer et;
  uip_ip4addr_t ip4;
  
  PROCESS_BEGIN();

  // Convert server address (IPv4) to uip_ipaddr_t format for Contiki
  const char *server_ip = "47.102.103.1";
  if(uiplib_ip4addrconv(server_ip, &ip4) == 0) {
    printf("Invalid IP address format\n");
    PROCESS_EXIT();
  }

  uip_ipaddr_copy(&server_ipaddr, &ip4);

  client_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  if(client_conn == NULL) {
    printf("No UDP connection available, exiting process\n");
    PROCESS_EXIT();
  }
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      char buf[20];
      sprintf(buf, "Device state: work");
      uip_udp_packet_send(client_conn, buf, strlen(buf));
      etimer_reset(&et);
    }
  }

  PROCESS_END();
}
