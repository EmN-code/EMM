#include "os/dev/leds.h"
#include "net/netstack.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/tcp-socket.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct etimer send_timer;
static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  printf("Starting UDP client\n");

  uip_ip6addr(&server_ipaddr, 0x47, 0x102, 0x103, 0, 0, 0, 0, 0x1);

  udp_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  if(udp_conn == NULL) {
    printf("No UDP connection available, exiting process\n");
    PROCESS_EXIT();
  }

  etimer_set(&send_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));

    printf("Sending device state to server\n");
    uip_udp_packet_send(udp_conn, DEVICE_STATE, strlen(DEVICE_STATE));

    etimer_reset(&send_timer);
  }

  PROCESS_END();
}