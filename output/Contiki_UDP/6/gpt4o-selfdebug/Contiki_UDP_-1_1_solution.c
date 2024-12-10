#include <string.h>
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/ip/uip.h"
#include "net/ip/uip-debug.h"

#define UDP_PORT 20001

static struct simple_udp_connection udp_conn;
static char *device_status = "work";  // or "not work"

PROCESS(udp_client_process, "UDP client ");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  uip_ipaddr_t server_addr;

  PROCESS_BEGIN();

  /* Initialize the UDP connection */
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  /* Create the server address */
  uip_ip6addr(&server_addr, 0x2f3, 0x6600, 0xabab, 0, 0, 0, 0xac24, 0x6703);

  /* Setup periodic timer to check the status and send a message every 5 seconds */
  etimer_set(&periodic_timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    /* Send device status to server */
    simple_udp_sendto(&udp_conn, device_status, strlen(device_status), &server_addr);

    /* Reset the timer */
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}