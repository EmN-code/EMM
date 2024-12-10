#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include "contiki.h"

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_IP "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  uip_ipaddr_t addr;

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

  etimer_set(&timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    uiplib_ipaddrconv(UDP_IP, &addr);

    const char *state_message = "work"; // Example state message

    simple_udp_sendto(&udp_conn, state_message, strlen(state_message) + 1, &addr);

    etimer_reset(&timer);
  }

  PROCESS_END();
}