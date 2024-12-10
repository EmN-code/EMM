#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (CLOCK_SECOND * 5)

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static struct uip_udp_conn *client_conn;
  static uip_ipaddr_t dest_ipaddr;
  static char buf[20];

  PROCESS_BEGIN();

  etimer_set(&timer, SEND_INTERVAL);

  uip_ipaddr(&dest_ipaddr, 47, 102, 103, 1);
  client_conn = udp_new(&dest_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);

  if (client_conn == NULL) {
    printf("No UDP connection, exiting process\n");
    PROCESS_EXIT();
  }

  udp_bind(client_conn, UIP_HTONS(0)); // Let the OS select a port

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    snprintf(buf, sizeof(buf), "%s", DEVICE_STATE);
    uip_udp_packet_send(client_conn, buf, strlen(buf));
    printf("Sent state to 47.102.103.1:20001\n");
    etimer_reset(&timer);
  }

  PROCESS_END();
}