#include "sys/etimer.h"
#include "net/rime/rime.h"
#include "contiki.h"

#include <stdio.h>

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct etimer timer;
static char* device_state = "work";

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct uip_udp_conn *conn;
  uip_ipaddr_t server_ip;

  PROCESS_BEGIN();

  // Set the time interval
  etimer_set(&timer, CLOCK_SECOND * 5);

  // Set server IP address and create connection
  uip_ipaddr(&server_ip, 47, 102, 103, 1);
  conn = udp_new(&server_ip, UIP_HTONS(20001), NULL);

  if(!conn) {
    printf("Failed to create UDP connection\n");
    PROCESS_EXIT();
  }

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);

    // Send UDP packet
    uip_udp_packet_send(conn, device_state, sizeof(device_state));
    printf("Sent device state: %s\n", device_state);
  }

  PROCESS_END();
}