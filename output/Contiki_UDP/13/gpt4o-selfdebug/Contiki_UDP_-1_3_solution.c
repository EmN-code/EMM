#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "lib/random.h"
#include <stdio.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define SERVER_IP "::ffff:47.102.103.1" // IPv4-mapped IPv6 address

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer timer;
  uip_ip6addr_t server_ipaddr;

  PROCESS_BEGIN();

  // Assign server address
  uip_ip6addr(&server_ipaddr, 0, 0, 0, 0, 0, 0xFFFF, 0x2F66, 0x6701);

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    char *str = "work";
    printf("Sending message: %s\n", str);
    simple_udp_sendto(&udp_conn, str, strlen(str) + 1, &server_ipaddr);
  }
  
  PROCESS_END();
}
