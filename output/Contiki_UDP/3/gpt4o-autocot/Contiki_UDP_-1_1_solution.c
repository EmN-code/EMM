#include "net/netstack.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();
  
  uip_ipaddr_t server_ipaddr;
  uip_ip6addr(&server_ipaddr, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 1);

  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  static struct etimer periodic_timer;
  etimer_set(&periodic_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    
    char buf[32];
    const char *device_state = "work";  // This should be determined by your actual device logic
    snprintf(buf, sizeof(buf), "Device status: %s", device_state);
    
    simple_udp_sendto(&udp_conn, buf, strlen(buf), &server_ipaddr);

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}