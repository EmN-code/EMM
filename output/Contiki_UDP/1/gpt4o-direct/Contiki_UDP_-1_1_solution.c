#include "sys/etimer.h"
#include "contiki.h"
#include "contiki-net.h"

#include <string.h> // for memcpy and strlen
#include <stdio.h>  // for printf

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static struct uip_udp_conn *client_conn;
  static uip_ipaddr_t dest_ipaddr;
  static char device_state[] = "work"; // or "not work"
  
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  uip_ipaddr(&dest_ipaddr, 47,102,103,1);

  client_conn = udp_new(&dest_ipaddr, UIP_HTONS(SERVER_PORT), NULL);
  if(client_conn == NULL) {
    printf("Failed to create UDP connection\n");
    PROCESS_EXIT();
  }
  printf("Created a connection with the server: %s:%d\n", SERVER_IP, SERVER_PORT);

  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &timer) {
      uip_udp_packet_send(client_conn, device_state, strlen(device_state));
      printf("Sent state: %s\n", device_state);
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}