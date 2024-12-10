#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *client_conn;
static char device_state[] = "work"; // Update to "not work" as necessary

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  printf("Starting UDP client process\n");

  // Set server IP address
  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
  client_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);

  if (client_conn == NULL) {
    printf("No UDP connection available, exiting the process\n");
    PROCESS_EXIT();
  }

  udp_bind(client_conn, UIP_HTONS(0));

  while(1) {
    etimer_set(&timer, SEND_INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    printf("Sending device state: %s\n", device_state);
    uip_udp_packet_send(client_conn, device_state, strlen(device_state));
  }

  PROCESS_END();
}