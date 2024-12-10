#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"

#include "contiki.h"
#include <stdio.h>

PROCESS(nrf52840_udp_client_process, "nRF52840 UDP Client Process");
AUTOSTART_PROCESSES(&nrf52840_udp_client_process);

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;

PROCESS_THREAD(nrf52840_udp_client_process, ev, data)
{
  static struct etimer timer;
  static char state_message[] = "Device state: working"; // assuming device is in "working" state for demonstration

  PROCESS_BEGIN();

  // Set the server IP address
  uiplib_ipaddrconv("47.102.103.1", &server_ipaddr);

  // New connection with remote host
  client_conn = uip_udp_new(&server_ipaddr, UIP_HTONS(20001));

  if (client_conn == NULL) {
    printf("No UDP connection available, exiting process\n");
    PROCESS_EXIT();
  }

  // Set remote port
  uip_udp_bind(client_conn, UIP_HTONS(3000)); // Local port

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    printf("Sending UDP message: %s\n", state_message);

    uip_udp_packet_send(client_conn, state_message, sizeof(state_message));
  }

  PROCESS_END();
}
