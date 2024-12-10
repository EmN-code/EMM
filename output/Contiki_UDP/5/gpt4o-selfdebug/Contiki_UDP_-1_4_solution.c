#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *client_conn;
static char device_state[12] = "work";

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
  client_conn = uip_udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

  if(client_conn == NULL) {
    printf("Unable to create a new UDP connection.");
    PROCESS_EXIT();
  }

  etimer_set(&timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    printf("Sending device state: %s\n", device_state);

    // uip_udp_packet_send(client_conn, device_state, strlen(device_state));
    uip_udp_send(strlen(device_state)); // Corrected function
    memcpy(uip_appdata, device_state, strlen(device_state) + 1); // Copy data to buffer

    etimer_reset(&timer);
  }

  PROCESS_END();
}