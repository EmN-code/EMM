#include <net/ip/uip.h>
#include <stdio.h>

static struct uip_udp_conn *connection;
static uip_ipaddr_t server_ipaddr;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static char buf[20];

  PROCESS_BEGIN();

  // Convert the IP address from string to uip_ipaddr_t
  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
  connection = uip_udp_new(&server_ipaddr, UIP_HTONS(20001));
  
  if(!connection) {
    printf("Failed to establish UDP connection\n");
    PROCESS_EXIT();
  }
  
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    sprintf(buf, "Device is %s", "working");
    uip_udp_packet_send(connection, buf, strlen(buf));
    etimer_reset(&timer);
  }

  PROCESS_END();
}
