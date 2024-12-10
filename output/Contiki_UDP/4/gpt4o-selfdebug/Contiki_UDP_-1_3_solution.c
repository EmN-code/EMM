/* This program is designed to run on the NRF52840 platform using Contiki. It connects to a UDP server to send the device state every five seconds. */

#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define LOCAL_UDP_PORT 30001
#define INTERVAL 5 * CLOCK_SECOND

static struct etimer periodic_timer;
static struct uip_udp_conn *client_conn;

PROCESS(udp_client_process, "UDP client example");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static char message[20];
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  printf("UDP client process started\n");

  // Set the remote server address
  uip_ipaddr(&dest_ipaddr, 47, 102, 103, 1);

  // New UDP connection with remote server
  client_conn = udp_new(&dest_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(LOCAL_UDP_PORT));

  etimer_set(&periodic_timer, INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    // Make the message
    snprintf(message, sizeof(message), "Device state: %s", "work");

    // Send UDP packet
    uip_udp_packet_send(client_conn, message, strlen(message));
    printf("Sent UDP message: %s\n", message);

    // Reset the timer
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}