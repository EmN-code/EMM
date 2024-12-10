#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define CLIENT_SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data){
  static struct etimer timer;
  static struct simple_udp_connection udp_conn;
  static char payload[20];
  uip_ipaddr_t server_ipaddr;
  
  PROCESS_BEGIN();
  
  uip_ip6addr(&server_ipaddr, 0x2f8e, 0x1fe, 0xa9a, 0x474, 0x1, 0, 0, 1); /* IPv6 representation for IPv4 47.102.103.1 */

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, 
                      UDP_SERVER_PORT, NULL);

  etimer_set(&timer, CLIENT_SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    sprintf(payload, "Device state: %s", "work");

    simple_udp_sendto(&udp_conn, payload, strlen(payload) + 1, &server_ipaddr);

    etimer_reset(&timer);
  }

  PROCESS_END();
}